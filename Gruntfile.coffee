module.exports = (grunt) ->
  grunt.initConfig
    pkg: grunt.file.readJSON('package.json')

    coffee:
      glob_to_multiple:
        expand: true
        cwd: 'src'
        src: ['*.coffee']
        dest: 'lib'
        ext: '.js'

    coffeelint:
      options:
        max_line_length:
          level: 'ignore'

      src: ['src/**/*.coffee']
      test: ['spec/**/*.coffee']

    cpplint:
      files: ['src/**/*.cc']
      reporter: 'spec'
      verbosity: 1
      filters:
        build:
          include: false
          namespaces: false
        legal:
          copyright: false
        readability:
          braces: false

    shell:
      rebuild:
        command: 'npm build .'
        options:
          stdout: true
          stderr: true
          failOnError: true

      test:
        command: 'node node_modules/jasmine-focused/bin/jasmine-focused --captureExceptions --coffee spec/'
        options:
          stdout: true
          stderr: true
          failOnError: true

  grunt.loadNpmTasks('grunt-contrib-coffee')
  grunt.loadNpmTasks('grunt-shell')
  grunt.loadNpmTasks('grunt-coffeelint')
  grunt.loadNpmTasks('node-cpplint')
  grunt.registerTask('lint', ['coffeelint', 'cpplint'])
  grunt.registerTask('default', ['coffee', 'lint', 'shell:rebuild'])
  grunt.registerTask('test', ['default', 'shell:test'])
  grunt.registerTask 'clean', ->
    rm = require('rimraf').sync
    rm('lib')
    rm('build')
