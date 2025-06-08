executable('stream_service',
  ['src/main.cpp', 'src/redis_subscriber.cpp', 'src/tick_parser.cpp'],
  include_directories : include_directories('include'),
  dependencies : [
    dependency('spdlog'),
    dependency('fmt'),
    dependency('boost'),
    dependency('hiredis'),
    dependency('rapidjson')
  ]
)
