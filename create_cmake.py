import os, sys, shutil, stat


###########################################################################
def on_rm_error(*args):
    '''
    In case the file or directory is read-only and we need to delete it
    this function will help to remove 'read-only' attribute
    :param args: (func, path, exc_info) yuple
    '''
    # path contains the path of the file that couldn't be removed
    # let's just assume that it's read-only and unlink it.
    _, path, _ = args
    os.chmod(path, stat.S_IWRITE)
    os.unlink(path)


def main(argv):
    try:
        if os.path.isdir('build-cmake'):
            shutil.rmtree('build-cmake', onerror=on_rm_error)
        os.mkdir('build-cmake')
        os.chdir('build-cmake')
        return os.system('cmake ..')
    except Exception as e:
        print('Exception: {0}'.format(e))
    # return non-zero if here
    return 1

###################################
if __name__ == "__main__":
    sys.exit(main(sys.argv))
