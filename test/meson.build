
foreach t : ['boolean', 'string', 'integer', 'null', 'list']
    name = 'nested.' + t
    filename = name + '.test.cpp'
    
    x = executable(
        name,
        filename,
        include_directories: includes,
        link_with: lib,
        install: false)

    test(name, x, args: files('aux/users.god'))
endforeach

plain_tests = [
    'integer-max',
    'integer-min',
    'double-max',
    'double-min'
]

foreach t : plain_tests
    name = t
    filename = t + '.test.cpp'
    x = executable(
        name,
        filename,
        include_directories: includes,
        link_with: lib,
        install: false)
    test(name, x)
endforeach

tests = [
    'matrix',
    'deep-matrix',
    'mixed',
    'empty-list',
    'empty-map',
    'empty-nested-list',
    'illegal-identifier',
    'identifier-in-list',
    'termination-in-list',
    'missing-termination',
    'deep-nested-query',
    'no-closing-brace',
    'no-opening-brace',
    'no-value',
    'no-clobber',
    'overwrite'
]

foreach t : tests
    name = t
    filename = t + '.test.cpp'
    input_file = 'aux/' + name + '.god'
    x = executable(
        name,
        filename,
        include_directories: includes,
        link_with: lib,
        install: false)

    test(name, x, args: files(input_file))
endforeach