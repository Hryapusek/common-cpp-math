import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import copy, rmdir


class CppMathRecipe(ConanFile):
    name = "cpp-math"
    version = "0.0.1"
    description = "This library contains some useful math functions"

    settings = "os", "compiler", "build_type"

    exports_sources = "*"

    options = {
        'shared': [True, False],
        'tests': [True, False],
    }

    default_options = {
        'shared': True,
        'tests': False
    }

    @property
    def _min_cppstd(self):
        return '14'

    def requirements(self):
        if self.options.tests:
            self.requires("catch2/[>=3.7.0]")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def configure(self):
        pass

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator = "Ninja")
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables[f"BUILD_{self.name}_TEST_EXECUTABLE"] = self.options.tests
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.libs = ["cpp-math"]
        self.cpp_info.set_property("cmake_file_name", "cpp-math")
        self.cpp_info.set_property("cmake_target_name", "cpp-math::cpp-math")
