from conans import ConanFile, CMake, tools

class MooveConan(ConanFile):
    name = "Moove"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("boost/1.69.0@conan/stable")
        if self.settings.os == "Windows":
            self.requires("winflexbison/2.5.21@bincrafters/stable")

    def configure(self):
        self.options["boost"].header_only = True

