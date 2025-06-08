project('order-router-service', 'cpp', default_options: ['cpp_std=c++20'])

srcs = ['main.cpp', 'src/order_router.cpp']

deps = dependency('fmt'),
       dependency('spdlog'),
       dependency('boost'),
       dependency('hiredis'),
       dependency('rapidjson')

executable('order-router-service', srcs,
  include_directories: include_directories('include'),
  dependencies: deps,
  install: true,
)
