flex_exe = find_program('flex', native: true)
bison_exe = find_program('bison', native: true)

flex_gen = generator(flex_exe,
    output: '@BASENAME@.yy.c',
    arguments: ['--nowarn', '-P@BASENAME@_yy', '@EXTRA_ARGS@', '-o@OUTPUT@', '@INPUT@']
)

bison_gen = generator(bison_exe,
    output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments: ['-d', '-p', '@BASENAME@_yy', '@EXTRA_ARGS@', '-o', '@BUILD_DIR@/@BASENAME@.tab.c', '@INPUT@']
)

generated_sources = [
    flex_gen.process('spec.l', 'tlcf.l'),
    bison_gen.process('spec.y', 'tlcf.y')
]

makelcf_exe = executable('makelcf',
    include_directories: twlsdk_includes,
    sources: [
        'container.c',
        'createlcf.c',
        'defval.c',
        'makelcf.c',
        'misc.c',
        generated_sources
    ],
    c_args: '-DSDK_WIN32',
    native: true
)
