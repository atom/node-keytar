{
  'targets': [
    {
      'target_name': 'keytar',
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [
        'src/main.cc',
        'src/keytar.h',
      ],
      'conditions': [
        ['OS=="mac"', {
          'sources': [
            'src/keytar_mac.cc',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
            ],
          },
        }],
        ['OS=="win"', {
          'sources': [
            'src/keytar_win.cc',
          ],
          'msvs_disabled_warnings': [
            4267,  # conversion from 'size_t' to 'int', possible loss of data
            4530,  # C++ exception handler used, but unwind semantics are not enabled
            4506,  # no definition for inline function
          ],
        }],
        ['OS not in ["mac", "win"]', {
          'sources': [
            'src/keytar_posix.cc',
          ],
          'cflags': [
            '<!(pkg-config --cflags libsecret-1)',
            '<!(pkg-config --cflags glib-2.0)',
            '-Wno-missing-field-initializers',
          ],
          'link_settings': {
            'ldflags': [
              '<!(pkg-config --libs-only-L --libs-only-other libsecret-1)',
              '<!(pkg-config --libs-only-L --libs-only-other glib-2.0)',
            ],
            'libraries': [
              '<!(pkg-config --libs-only-l libsecret-1)',
              '<!(pkg-config --libs-only-l glib-2.0)',
            ],
          },
        }],
      ],
    }
  ]
}
