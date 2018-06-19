#!/bin/python
# coding: utf-8
import os
import sys
import shutil
import zipfile
import time

Jobs = 3
ftime = time.mktime((1990,11,21,9,0,0,0,0,-1))

def fexist(path):
    if(not os.path.exists(path)):
        print("\"%s\" isn't exists." % path)
        return False
    return True

def chk_components():
    print('--- chk_components()')
    if(not fexist('./components/asar.dll'))    : return False
    if(not fexist('./components/trasm_b.exe')) : return False
    return True

def RemoveFile(path):
    if(os.path.exists(path)) : os.remove(path)

def build():
    print('--- build()')
    # clean previous make files
    os.system('make clean')
    shutil.rmtree('./CMakeFiles', ignore_errors=True)
    RemoveFile('Makefile')
    RemoveFile('CMakeCache.txt')
    RemoveFile('cmake_install.cmake')

    # cmake
    cmakecmd = [
            'cmake',
            '..',
            '-DCMAKE_TOOLCHAIN_FILE=../toolchain/mingw-x86.cmake',
            '-DJANSSON_WITHOUT_TESTS=ON',
            '-DJANSSON_BUILD_SHARED_LIBS=ON',
            '-DJANSSON_INSTALL=OFF',
            '-DJANSSON_BUILD_DOCS=OFF'
            ]
    if(os.system(' '.join(cmakecmd))) : return False

    # make
    if(os.system("cmake --build . -- -j%d" % Jobs)) : return False
    return True

def makedir(dest):
    print('mkdir "%s" ... '%dest, end="")
    try:
        os.mkdir(dest)
    except FileExistError:
        print('failed')
        return False
    print('succeeded')
    return True


def make_dirs(root_dir):
    print('--- make_dirs()')
    shutil.rmtree(root_dir, ignore_errors=True)
    if(not makedir(root_dir))               : return False
    if(not makedir(root_dir+"/components")) : return False
    if(not makedir(root_dir+"/asm"))        : return False
    return True

def Copy(src, ddir, dest):
    con = ddir+dest
    print('cp "%s" => "%s" ... '%(src,con), end="")
    try:
        shutil.copy(src, con)
    except:
        print('failed')
        return False
    print('succeeded')
    return True

def CpDir(src, ddir, dest):
    con = ddir+dest
    print('cpdir "%s" => "%s" ... '%(src,con), end="")
    try:
        shutil.copytree(src, con)
    except:
        print('failed')
        return False
    print('succeeded')
    return True

def copy_files(root_dir):
    print('--- copy_files()')

    cpfilelist = [
            ('./giepy.exe', '/'),
            ('./piee.exe', '/'),
            ('./components/mewthree.dll', '/components/'),
            ('./components/jansson.dll', '/components/'),
            ('./components/asar.dll', '/components/'),
            ('./components/trasm_b.exe', '/components/'),
            ('../LICENSE', '/'),
            ('../asm/giepy.asm', '/asm/'),
            ('../asm/giepy.asm', '/asm/'),
            ('../asm/smwlibs.asm', '/asm/'),
            ('../doc/readme.txt', '/'),
            ('../doc/readme_jp.txt', '/'),
            ('../samples/list.txt', '/'),
            ('../scripts/InsertPrompt.bat', '/'),
            ('../scripts/giepy.bat', '/'),
            ('../scripts/tee.bat', '/components/'),
            ]
    for cpitem in cpfilelist:
        if(not Copy(cpitem[0], root_dir, cpitem[1])): return False

    cpdirlist = [
            ('../asm/giepy', '/asm/giepy'),
            ('../asm/include', '/asm/include'),
            ('../doc/asar_licenses/', '/asar_licenses'),
            ('../samples/libraries', '/libraries'),
            ('../samples/sprites', '/sprites'),
            ('../samples/extendedes', '/extendedes'),
            ('../samples/clusters', '/clusters'),
            ]
    for cpitem in cpdirlist:
        if(not CpDir(cpitem[0], root_dir, cpitem[1])): return False

    return True

def update_timestamp(root_dir):
    print('--- update_timestamp()')

    print('Updating time stamps ... ', end="")
    try:
        os.utime(root_dir,(ftime,ftime))
        for dirPath, dirNames, fileNames in os.walk(root_dir):
            for fname in fileNames:
                fp = os.path.join(dirPath,fname)
                os.utime(fp,(ftime,ftime))
            for dname in dirNames:
                fp = os.path.join(dirPath,dname)
                os.utime(fp,(ftime,ftime))
    except:
        print('failed')
        return False

    print('succeeded')
    return True

def zip_dir(root_dir):
    print('--- zip_dir()')
    zipItems = []
    zipName = '%s.zip'%os.path.basename(root_dir)

    print('creating "%s" ... '%zipName, end="")
    try:
        for dirPath, dirNames, fileNames in os.walk(root_dir):
            for fname in fileNames:
                fp = os.path.join(dirPath,fname)
                if(fp==zipName):
                    continue
                content = os.path.relpath(fp,os.path.dirname(root_dir))
                zipItems.append((fp,content))
            for dname in dirNames:
                fp = os.path.join(dirPath,dname)
                content = os.path.relpath(fp,os.path.dirname(root_dir)) + os.path.sep
                zipItems.append((fp,content))
    
        zipFile = zipfile.ZipFile(zipName, 'w', zipfile.ZIP_DEFLATED)
        for fp,name in zipItems:
            zipFile.write(fp,name)
        zipFile.close()
        os.utime(zipName, (ftime,ftime))

    except:
        print('failed')
        return False

    print('succeeded')
    return True


def clean_files(root_dir):
    print('--- clean_files()')
    shutil.rmtree(root_dir, ignore_errors=True)
    return True


def main(argv):
    if (1 == len(argv)):
        print('Usage: %s <version>' % argv[0])
        return -1

    ver = "giepy_v%s" % argv[1]
    strdir = os.path.dirname(argv[0])
    if(len(strdir)) : os.chdir(os.path.dirname(argv[0]))

    if(not chk_components())      : return -1
    if(not build())               : return -1
    if(not make_dirs(ver))        : return -1
    if(not copy_files(ver))       : return -1
    if(not update_timestamp(ver)) : return -1
    if(not zip_dir(ver))          : return -1
    if(not clean_files(ver))      : return -1

    print('done.')

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))

