from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class BasicHttpServerRecipe(ConanFile):
    name = "basic_http_server"
    version = "0.0.0"
    package_type = "application"

    # Optional metadata
    license = "<Put the package license here>"
    author = "VonBrank vonbrank@outlook.com"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of basic_http_server package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def requirements(self):
        self.requires("boost/1.84.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    
