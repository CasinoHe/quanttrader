# build dependent thirdparties and build project

import argparse
import sys
import os
import subprocess

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

    def _run_command(self, workdir, command, args):
        if not os.path.exists(workdir):
            raise RuntimeError("Runcommand %s Workdir not found: %s" % (command, workdir))

        cwd = os.getcwd()
        os.chdir(workdir)
        print("Starting Run command: %s %s under %s" % (command, " ".join(args), workdir))
        process = subprocess.Popen([command] + args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True)

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

    def _build_server_dependencies(self):
        print("Starting build dependencies for quanttrader server.")
        triplet = "x64-windows-static" if sys.platform == "win32" else "x64-linux"
        args = ["install", "--triplet", triplet]
        if self.args.dry_run:
            args.append("--dry-run")
        if not self._run_command(self.server_root, self.vcpkg_path, args):
            raise RuntimeError("Failed to install dependencies")
        print("Build dependencies for quanttrader server finished.")

    def _build_client_dependencies(self):
        print("Starting build dependencies for quanttrader client.")
        triplet = "x64-windows-static" if sys.platform == "win32" else "x64-linux"
        args = ["install", "--triplet", triplet]
        if self.args.dry_run:
            args.append("--dry-run")
        if not self._run_command(self.client_root, self.vcpkg_path, args):
            raise RuntimeError("Failed to install dependencies")
        print("Build dependencies for quanttrader client finished.")

    def build_dependencies(self):
        if not os.path.exists(self.vcpkg_path):
            raise RuntimeError("Vcpkg path not found: %s" % self.vcpkg_path)

        # install dependencies
        if self.args.build_type == "server":
            self._build_server_dependencies()
        elif self.args.build_type == "client":
            self._build_client_dependencies()

    def build(self):
        self.prepare_vcpkg()
        self.build_dependencies()


def parse_args(args):
    if args is None:
        args = sys.argv[1:]

    parser = argparse.ArgumentParser(description="Build project", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    sub_parser = parser.add_subparsers(help="sub-command help", dest="command")

    build_dependencies_parser = sub_parser.add_parser(BUILD_DEPENDENCIES, help="Build dependencies")
    build_dependencies_parser.add_argument("--build-type", default="server", choices=["server", "client"], help="Build server or client dependencies, default is server")
    build_dependencies_parser.add_argument("--build-variant", default="Release", choices=["Release", "Debug"], help="Build type")
    build_dependencies_parser.add_argument("--force-rebuild", action="store_true", default=False, help="Force rebuild all dependencies")
    build_dependencies_parser.add_argument("--dry-run", action="store_true", default=False, help="Use vcpkg on --dry-run mode")

    return parser.parse_args(args), parser


def main():
    args, parser = parse_args(None)

    if args.command == BUILD_DEPENDENCIES:
        build_dependencies = BuildDependencies(args, parser)
        build_dependencies.build()
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
