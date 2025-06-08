executable('redis_service',
  ['src/main.cpp', 'src/redis_connection.cpp', 'src/order_updater.cpp', 'src/trade_broadcaster.cpp'],
  include_directories : include_directories('include'),
  dependencies : [
    dependency('spdlog'),
    dependency('fmt'),
    dependency('boost'),
    dependency('hiredis'),
    dependency('rapidjson')
  ]
)
