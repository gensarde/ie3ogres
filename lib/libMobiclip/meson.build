libMobiclip_srcs = files(
    'asm/YuvToArgb.s',
	'src/libMobiclip.cpp',
	'src/handle.cpp',
	'src/relocator.cpp',
	'src/streamfile.cpp',
	'src/streammemory.cpp',
	'asm/vx2Code.s'
)

libMobiclip_includes = include_directories(
    'asm', 'include'
)

public_includes += libMobiclip_includes

libMobiclip = static_library('Mobiclip',
    cpp_args: cpp_args,
    nasm_args: asm_args,
    include_directories: [
        public_includes,
        twlsdk_includes
    ],
    pic: false,
    sources: libMobiclip_srcs
)
