import pytest
import subprocess
import shutil
import os
import toml
import subprocess

test_folder = ".test"


def copy_files(exe_name):
    shutil.copyfile(f"{exe_name}/diagram.json", f"{test_folder}/diagram.json")
    shutil.copyfile(f"{exe_name}/test.yaml", f"{test_folder}/test.yaml")


def gen_wokwi_toml(exe_name):
    base_path = os.path.join("..", "build", exe_name)
    firmware_path = os.path.join(base_path, f"{exe_name}.uf2")
    elf_path = os.path.join(base_path, f"{exe_name}.elf")

    data = {"wokwi": {"version": 1, "firmware": firmware_path, "elf": elf_path}}
    os.makedirs(test_folder, exist_ok=True)
    toml_file_path = os.path.join(test_folder, "wokwi.toml")
    with open(toml_file_path, "w") as toml_file:
        toml.dump(data, toml_file)


def run_build(name):
    if not os.path.isdir("build"):
        os.makedirs("build", exist_ok=True)
        os.chdir("build")
        subprocess.run(
            "cmake ..",
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
    else:
        os.chdir("build")
    subprocess.run(
        f"make {name}",
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    os.chdir("..")


def run_test(name):
    print("Building projects")
    run_build(name)
    gen_wokwi_toml(name)
    copy_files(name)
    os.chdir(test_folder)
    command = "wokwi-cli --timeout 5000 --scenario test.yaml"
    process = subprocess.run(command, shell=True, stderr=subprocess.PIPE, text=True)
    os.chdir("..")
    if process.returncode != 0:
        return 1
    return 0


def test_code(name):
    assert run_test(name) == 0
