subdir('bfmt')
subdir('effect3d')
subdir('face2d')
subdir('font')
subdir('logic')
subdir('map2d')
subdir('model')
subdir('movie')
subdir('obj2d/rpg')
subdir('pic2d')
subdir('pic3d')
subdir('script')
subdir('sound')
subdir('spr')

prebuilt_files = [
    'DEBUG_BGM_LIST.txt',
    'DEBUG_MAPJUMP_LIST.TXT',
    'DEBUG_SCRIPT_LIST.txt',
    'EVENTINFO.TXT',
    'INAZUMA.INI',
    'MCSFILE.TXT',
    'ROM_TIME.TXT',
    'SCRIPTINFO.TXT',
    'SCRIPTINFOMAP.TXT'
]

foreach f : prebuilt_files
    nitrofs_files += fs.copyfile(f)
endforeach