libsyscall_asm = files(
    'asm/_secure.s'
)

libsyscall_includes = include_directories(
    'asm/include'
)

libsyscall = static_library('syscall',
    include_directories: [
        public_includes,
        libsyscall_includes
    ],
    pic: false,
    sources: libsyscall_asm
)
