project('liquidation-service', 'cpp', default_options: ['cpp_std=c++20'])

srcs = ['main.cpp', 'src/liquidation_service.cpp']

deps = dependency('fmt'),
       dependency('spdlog'),
       dependency('boost'),
       dependency('hiredis')

executable('liquidation-service', srcs,
  include_directories: include_directories('include'),
  dependencies: deps,
  install: true,
)
