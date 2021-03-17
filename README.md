# Install VCPKG
Install vcpkg somewhere (eg C:/vcpkg) and run its boostrap script etc

# Install Jansson
In your vcpkg directory, run `vcpkg install jansson:x64-windows`

# Configure cmake
Go back to this folder, and run this:

```
mkdir build
cd build
cmake --configure .. -DCMAKE_TOOLCHAIN_FILE=<PATH_TO_TOOLCHAIN_FILE_HERE> -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --configure .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows
```

# Build the project
Run `cmake --build .` inside the build folder

