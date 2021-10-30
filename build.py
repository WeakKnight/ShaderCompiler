import shutil
import os

dir_path = os.path.dirname(os.path.realpath(__file__))
print("Current Path is " + dir_path)

os.chdir(dir_path + "/build")
os.system('cmake -G "Visual Studio 16 2019" ../')

os.chdir(dir_path)
shutil.copyfile("thirdparty/slang/win64/slang.dll", "bin/slang.dll")
shutil.copyfile("thirdparty/slang/win64/dxcompiler.dll", "bin/dxcompiler.dll")
shutil.copyfile("thirdparty/slang/win64/dxil.dll", "bin/dxil.dll")

if os.path.isdir("bin/shaders"):
    shutil.rmtree("bin/shaders")
shutil.copytree("shaders", "bin/shaders")

