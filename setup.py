from distutils.core import setup, Extension

import os
import platform

OS = platform.system()

SRC_FOLDER = './src/'

os.system("./build.sh")

guava_sources =[ SRC_FOLDER + name for name in [
    'guava_conn.c',
    'guava_handler/guava_handler.c',
    'guava_handler/guava_handler_static.c',
    'guava_mime_type.c',
    'guava_request.c',
    'guava_response.c',
    'guava_router/guava_router.c',
    'guava_router/guava_router_mvc.c',
    'guava_router/guava_router_rest.c',
    'guava_router/guava_router_static.c',
    'guava_server.c',
    'guava_string.c',
    'guava_session/guava_session.c',
    'guava_session/guava_session_store_inmem.c',
    'guava_session/guava_session_store_file.c',
    'guava_cookie.c',
    'guava_memory.c',
]]

guava_module_sources = [ SRC_FOLDER + 'guava_module/' + name for name in [
    'controller.c',
    'cookie.c',
    'handler/handler.c',
    'handler/redirect.c',
    'handler/static.c',
    'module.c',
    'request.c',
    'response/response.c',
    'router/mvc.c',
    'router/rest.c',
    'router/router.c',
    'router/static.c',
    'server.c',
    'session.c',
]]

http_parser_include = ['deps/http-parser']
http_parser_files = ['deps/http-parser/http_parser.c']

libuv_include = ['deps/libuv/include']

compile_flags = ['-O0', '-ggdb', '-std=c99']

if OS == 'Linux':
    libraries = ['rt']
else:
    libraries = []

macros = [('HTTP_PARSER_STRICT', 1)]

if os.environ.get('GUAVA_DEBUG', None):
    macros += [('GUAVA_MEM_DEBUG', 1)]

guava_module = Extension('guava',
                         sources=guava_sources + http_parser_files + guava_module_sources,
                         include_dirs=['./include/'] + http_parser_include + libuv_include,
                         libraries=[] + libraries,
                         define_macros=macros,
                         extra_compile_args=compile_flags,
                         extra_objects=['./deps/libuv/.libs/libuv.a'])

setup(name='guava',
      version='0.1',
      description='Guava - A lightweight and high performance web framework for Python written in C',
      author='Rock Lee',
      author_email='insfocus@gmail.com',
      maintainer='Rock Lee',
      maintainer_email='insfocus@gmail.com',
      url='https://github.com/flatpeach/guava',
      download_url='https://github.com/flatpeach/guava/tarball/0.1a',
      ext_modules=[
          guava_module,
      ],
      license='BSD'
)
