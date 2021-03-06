Writing service definitions from scratch
========================================

By now, each time we did a service definition, we extended the `kgr::Service` or the `kgr::SingleService`. But we are not limited to that.

Writing a service definition is pretty easy. You need to define two function for it to work

 * `construct`
 * `forward`

The `construct` function is static and retuning the service definition itself. It's a factory method. It can take any other service definition as parameter. This is where dependencies are resolved.
The `forward` function is taking no parameters and returing the service. The return type of this function define how your service should be injected. This function may invalidate the service definition.

Normally, your service definition contains your service.

A tipical service definition look like this:

    struct FileManagerService {
        static FileManagerService construct() {
            return {};
        }
        
        FileManager forward() {
            return std::move(fm);
        }
        
    private:
        FileManager fm;
    };

If the class `FileManager` has dependencies, you can add them in the construct as parameters:

    struct FileManagerService {
        static FileManagerService construct(NotificationService& ns, ClownMasterService cms) {
            return { FileManager{ns.forward(), cms.forward()} };
        }
        
        // return as move, invalidating the service definition is okay.
        FileManager forward() {
            return std::move(fm);
        }
        
    private:
        FileManager fm;
    };
    
Note that single services must be received as reference, like `NotificationService&` in this case. By definition, a Single must not be copied.
The container will call construct with the right set of parameter.

### Singles

There's two steps to make `FileManagerService` single. First, we need to make our struct inherit from `kgr::Single`. The second is to adapt the forward function to be virtual and not invalidate the contained service.

Note: single service can forward as copy too, but you rarely do that. Returing a reference or pointer is a much better idea when it comes to single service.

So let's change our sevice to a single:

    struct FileManagerService : Single {
        static FileManagerService construct(NotificationService& ns, ClownMasterService cms) {
            return { FileManager{ns.forward(), cms.forward()} };
        }
        
        // return as pointer, must not invalidate the service. Must be virtual.
        virtual FileManager* forward() {
            return &fm;
        }
        
    private:
        FileManager fm;
    };

Why should it be virtual? Remember the `Override` feature? To achieve this behaviour, the container has to generate a service definition that inherit yours and use the overrider's `forward` method. This seem complicated, and in some way it is. But at least, you don't have to deal with this. All you have to do is to make your `forward` method virtual. Simple!
