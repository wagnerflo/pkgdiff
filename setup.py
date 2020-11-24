from setuptools import setup,Extension
from subprocess import check_output

def pkgconfig(*args):
    return check_output(('pkg-config',) + args, text=True).strip().split()

setup(
    name='pkgdiff',
    description='Compare FreeBSD pkg files by metadata.',
    version='0.1',
    author='Florian Wagner',
    author_email='florian@wagner-flo.net',
    url='https://github.com/wagnerflo/pkgdiff',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Intended Audience :: System Administrators',
        'License :: OSI Approved :: Apache Software License',
        'Operating System :: POSIX :: BSD :: FreeBSD',
        'Programming Language :: C',
        'Programming Language :: Python :: 3 :: Only',
        'Topic :: System :: Archiving :: Packaging',
        'Topic :: System :: Systems Administration',
    ],
    ext_modules=[
        Extension(
            'pkgdiff',
            sources=[
                'python.c',
                'pkgdiff.c',
            ],
            extra_compile_args=[
                *pkgconfig('--cflags', 'pkg'),
            ],
            extra_link_args=[
                '-Wl,-Bstatic', '-lmba', '-Wl,-Bdynamic',
                *pkgconfig('--libs', 'pkg'),
            ],
        ),
    ],
)
