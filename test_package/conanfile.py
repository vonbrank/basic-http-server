import os
from conan import ConanFile
from conan.tools.build import can_run


class basic_http_serverTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def test(self):
        if can_run(self):
            self.run("basic_http_server", env="conanrun")
