# build dependent thirdparties and build project

import argparse
import sys
import os
import subprocess
import shutil

BUILD_DEPENDENCIES = "build_dependencies"
BUILD_SERVER = "build_server"
VCPKG_PATH = "thirdparties/vcpkg"


class BuildDependencies(object):
    def __init__(self, args, parser=None):
        self.args = args
        self.parser = parser

        self.init_project_path()

    def init_project_path(self):
        self.project_root = os.path.abspath(os.path.dirname(__file__))
        self.server_root = os.path.abspath(os.path.join(self.project_root, "server"))
        self.client_root = os.path.abspath(os.path.join(self.project_root, "client"))
        self.vcpkg_dir = os.path.abspath(os.path.abspath(os.path.join(os.path.dirname(__file__), VCPKG_PATH)))
        # detect platform
        if sys.platform == "win32":
            self.vcpkg_path = os.path.join(self.vcpkg_dir, "vcpkg.exe")
        else:
            self.vcpkg_path = os.path.join(self.vcpkg_dir, "vcpkg")

    def _run_command(self, workdir, command, args, env=None):
        if not os.path.exists(workdir):
            raise RuntimeError("Runcommand %s Workdir not found: %s" % (command, workdir))

        cwd = os.getcwd()
        os.chdir(workdir)
        print("Starting Run command: %s %s under %s" % (command, " ".join(args), workdir))
        process = subprocess.Popen([command] + args, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, text=True, shell=True, env=env)

        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                print(output.strip())

        stderr_output = process.stderr.read()
        os.chdir(cwd)
        if process.returncode != 0:
            print("Failed to run command: %s %s" % (command, " ".join(args)))
            print("stderr: %s" % stderr_output)
            return False
        else:
            print("Run command success: %s %s" % (command, " ".join(args)))
            return True

    def prepare_vcpkg(self):
        if not os.path.exists(self.vcpkg_dir):
            raise RuntimeError("Vcpkg path not found: %s" % self.vcpkg_dir)

        print("Build dependencies with vcpkg path: %s" % self.vcpkg_dir)

        def build_vcpkg():
            if sys.platform == "win32":
                return self._run_command(self.vcpkg_dir, "bootstrap-vcpkg.bat", [])
            else:
                return self._run_command(self.vcpkg_dir, "bootstrap-vcpkg.sh", [])

        if not os.path.exists(self.vcpkg_path):
            # compile vcpkg
            if not build_vcpkg():
                raise RuntimeError("Failed to compile vcpkg")
        else:
            if self.args.force_rebuild:
                # update vcpkg
                if not build_vcpkg():
                    raise RuntimeError("Failed to update vcpkg")
            else:
                print("Vcpkg already exists, skip build")

    def build_dependencies(self):
        if not os.path.exists(self.vcpkg_path):
            raise RuntimeError("Vcpkg path not found: %s" % self.vcpkg_path)

        if self.args.build_type == "server":
            workdir = self.server_root
        elif self.args.build_type == "client":
            workdir = self.client_root
        else:
            raise RuntimeError("Unknown build type: %s" % self.args.build_type)

        print(f"Starting build dependencies for quanttrader {self.args.build_type}.")

        if self.args.triplet:
            triplet = self.args.triplet
        else:
            triplet = "x64-windows-static-release" if sys.platform == "win32" else "x64-linux-release"

        # set build variant
        env = os.environ.copy()
        if self.args.build_variant == "Release":
            env["VCPKG_BUILD_TYPE"] = "Release"
        elif self.args.build_variant == "Debug":
            env["VCPKG_BUILD_TYPE"] = "Debug"

        args = ["install", "--triplet", triplet, "--vcpkg-root", self.vcpkg_dir,
                "--x-install-root", os.path.join(workdir, "build", "vcpkg_installed"),
                "--clean-buildtrees-after-build", "--clean-packages-after-build"]
        if self.args.dry_run:
            args.append("--dry-run")
        if not self._run_command(workdir, self.vcpkg_path, args, env=env):
            raise RuntimeError("Failed to install dependencies")

        print(f"Build dependencies for quanttrader {self.args.build_type} finished.")

    def build(self):
        self.prepare_vcpkg()
        self.build_dependencies()


class BuildServer(object):
    def __init__(self, args, parser=None):
        self.args = args
        self.parser = parser

        self.init_project_path()

    def _run_command(self, workdir, command, args, env=None):
        if not os.path.exists(workdir):
            raise RuntimeError("Runcommand %s Workdir not found: %s" % (command, workdir))

        cwd = os.getcwd()
        os.chdir(workdir)
        print("Starting Run command: %s %s under %s" % (command, " ".join(args), workdir))
        process = subprocess.Popen([command] + args, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, text=True, shell=True, env=env)

        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                print(output.strip())

        stderr_output = process.stderr.read()
        os.chdir(cwd)
        if process.returncode != 0:
            print("Failed to run command: %s %s" % (command, " ".join(args)))
            print("stderr: %s" % stderr_output)
            return False
        else:
            print("Run command success: %s %s" % (command, " ".join(args)))
            return True

    def init_project_path(self):
        self.project_root = os.path.abspath(os.path.dirname(__file__))
        self.server_root = os.path.abspath(os.path.join(self.project_root, "server"))
        self.vcpkg_dir = os.path.abspath(os.path.abspath(os.path.join(os.path.dirname(__file__), VCPKG_PATH)))

    def configure_server(self):
        print("Starting build quanttrader server.")

        if self.args.triplet:
            triplet = self.args.triplet
        else:
            triplet = "x64-windows-static-release" if sys.platform == "win32" else "x64-linux-release"

        if self.args.enable_install:
            manifest_install = "ON"
        else:
            if not os.path.exists(os.path.join(self.server_root, "build", "vcpkg_installed")):
                manifest_install = "ON"
            else:
                manifest_install = "OFF"

        variant = self.args.build_variant

        args = [
            f"-DCMAKE_TOOLCHAIN_FILE={self.vcpkg_dir}/scripts/buildsystems/vcpkg.cmake",
            f"-DVCPKG_MANIFEST_ROOT={self.server_root}",
            f"-DVCPKG_TARGET_TRIPLET={triplet}",
            f"-DVCPKG_MANIFEST_INSTALL={manifest_install}",
            f"-DCMAKE_BUILD_TYPE={variant}",
            f"{self.server_root}"
        ]

        workdir = os.path.join(self.server_root, "build")
        if not os.path.exists(workdir):
            os.makedirs(workdir)

        command = "cmake"
        if not self._run_command(workdir, command, args):
            raise RuntimeError("Failed to install dependencies")

        return True

    def build_server(self):
        print(f"Starting build quanttrader server using {self.args.build_variant}.")
        workdir = os.path.join(self.server_root, "build")
        if not os.path.exists(workdir):
            raise RuntimeError("Workdir not found: %s" % workdir)

        args = ["--build", ".", "--config", self.args.build_variant]
        if not self._run_command(workdir, "cmake", args):
            raise RuntimeError("Failed to build quanttrader server")

    def clean_build(self):
        workdir = os.path.join(self.server_root, "build")
        if os.path.exists(workdir):
            print(f"Remove build directory: {workdir}")
            shutil.rmtree(workdir)

        return True

    def build(self):
        if self.args.clean:
            self.clean_build()
        else:
            self.configure_server()
            self.build_server()


def parse_args(args):
    if args is None:
        args = sys.argv[1:]

    parser = argparse.ArgumentParser(description="Build project", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    sub_parser = parser.add_subparsers(help="sub-command help", dest="command")

    build_dependencies_parser = sub_parser.add_parser(BUILD_DEPENDENCIES, help="Build dependencies")
    build_dependencies_parser.add_argument("--build-type", default="server", choices=["server", "client"], help="Build server or client dependencies, default is server")
    build_dependencies_parser.add_argument("--build-variant", default="Release", choices=["Release", "Debug"], help="Build type")
    build_dependencies_parser.add_argument("--force-rebuild", action="store_true", default=False, help="Force rebuild all dependencies")
    build_dependencies_parser.add_argument("--triplet", type=str, required=False, help="Set vcpkg triplet, example: x64-windows-static")
    build_dependencies_parser.add_argument("--dry-run", action="store_true", default=False, help="Use vcpkg on --dry-run mode")

    server_parser = sub_parser.add_parser(BUILD_SERVER, help="Build server")
    server_parser.add_argument("--build-variant", default="Release", choices=["Release", "Debug"], help="Build type")
    server_parser.add_argument("--disable_vcpkg", action="store_true", default=False, help="Disable vcpkg use system libraries")
    server_parser.add_argument("--enable-install", action="store_true", default=False, help="Enable auto install vcpkg dependencies")
    server_parser.add_argument("--triplet", type=str, required=False, help="Set vcpkg triplet, example: x64-windows-static")
    server_parser.add_argument("--clean", action="store_true", default=False, help="Clean build directory")

    return parser.parse_args(args), parser


def main():
    args, parser = parse_args(None)

    if args.command == BUILD_DEPENDENCIES:
        build_dependencies = BuildDependencies(args, parser)
        build_dependencies.build()
    elif args.command == BUILD_SERVER:
        build_server = BuildServer(args, parser)
        build_server.build()
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
