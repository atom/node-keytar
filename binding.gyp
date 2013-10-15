{
  'targets': [
    {
      'target_name': 'keytar',
      'sources': [
        'src/main.cc'
        'src/keytar.h'
      ],
      'conditions': [
        ['OS=="mac"', {
          'sources': [
            'src/keytar_mac.cc',
          ],
        }],
        ['OS=="win"', {
          'sources': [
            'src/keytar_win.cc',
          ],
        }],
      ],
    }
  ]
}
