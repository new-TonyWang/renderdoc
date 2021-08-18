bool JavaVMExt::LoadNativeLibrary(JNIEnv* env,
                                  const std::string& path,
                                  jobject class_loader,
                                  jclass caller_class,
                                  std::string* error_msg) {
  error_msg->clear();

  // See if we've already loaded this library.  If we have, and the class loader
  // matches, return successfully without doing anything.
  // TODO: for better results we should canonicalize the pathname (or even compare
  // inodes). This implementation is fine if everybody is using System.loadLibrary.
  SharedLibrary* library;
  Thread* self = Thread::Current();
  {
    // TODO: move the locking (and more of this logic) into Libraries.
    MutexLock mu(self, *Locks::jni_libraries_lock_);
    library = libraries_->Get(path);
  }
  void* class_loader_allocator = nullptr;
  std::string caller_location;
  {
    ScopedObjectAccess soa(env);
    // As the incoming class loader is reachable/alive during the call of this function,
    // it's okay to decode it without worrying about unexpectedly marking it alive.
    ObjPtr<mirror::ClassLoader> loader = soa.Decode<mirror::ClassLoader>(class_loader);

    ClassLinker* class_linker = Runtime::Current()->GetClassLinker();
    if (class_linker->IsBootClassLoader(soa, loader.Ptr())) {
      loader = nullptr;
      class_loader = nullptr;
      if (caller_class != nullptr) {
        ObjPtr<mirror::Class> caller = soa.Decode<mirror::Class>(caller_class);
        ObjPtr<mirror::DexCache> dex_cache = caller->GetDexCache();
        if (dex_cache != nullptr) {
          caller_location = dex_cache->GetLocation()->ToModifiedUtf8();
        }
      }
    }

    class_loader_allocator = class_linker->GetAllocatorForClassLoader(loader.Ptr());
    CHECK(class_loader_allocator != nullptr);
  }
  if (library != nullptr) {
    // Use the allocator pointers for class loader equality to avoid unnecessary weak root decode.
    if (library->GetClassLoaderAllocator() != class_loader_allocator) {
      // The library will be associated with class_loader. The JNI
      // spec says we can't load the same library into more than one
      // class loader.
      //
      // This isn't very common. So spend some time to get a readable message.
      auto call_to_string = [&](jobject obj) -> std::string {
        if (obj == nullptr) {
          return "null";
        }
        // Handle jweaks. Ignore double local-ref.
        ScopedLocalRef<jobject> local_ref(env, env->NewLocalRef(obj));
        if (local_ref != nullptr) {
          ScopedLocalRef<jclass> local_class(env, env->GetObjectClass(local_ref.get()));
          jmethodID to_string = env->GetMethodID(local_class.get(),
                                                 "toString",
                                                 "()Ljava/lang/String;");
          DCHECK(to_string != nullptr);
          ScopedLocalRef<jobject> local_string(env,
                                               env->CallObjectMethod(local_ref.get(), to_string));
          if (local_string != nullptr) {
            ScopedUtfChars utf(env, reinterpret_cast<jstring>(local_string.get()));
            if (utf.c_str() != nullptr) {
              return utf.c_str();
            }
          }
          if (env->ExceptionCheck()) {
            // We can't do much better logging, really. So leave it with a Describe.
            env->ExceptionDescribe();
            env->ExceptionClear();
          }
          return "(Error calling toString)";
        }
        return "null";
      };
      std::string old_class_loader = call_to_string(library->GetClassLoader());
      std::string new_class_loader = call_to_string(class_loader);
      StringAppendF(error_msg, "Shared library \"%s\" already opened by "
          "ClassLoader %p(%s); can't open in ClassLoader %p(%s)",
          path.c_str(),
          library->GetClassLoader(),
          old_class_loader.c_str(),
          class_loader,
          new_class_loader.c_str());
      LOG(WARNING) << *error_msg;
      return false;
    }
    VLOG(jni) << "[Shared library \"" << path << "\" already loaded in "
              << " ClassLoader " << class_loader << "]";
    if (!library->CheckOnLoadResult()) {
      StringAppendF(error_msg, "JNI_OnLoad failed on a previous attempt "
          "to load \"%s\"", path.c_str());
      return false;
    }
    return true;
  }