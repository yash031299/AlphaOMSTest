project('kline-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

boost_dep = dependency('boost', modules: ['asio'])
spdlog_dep = dependency('spdlog')

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/candle_aggregator.cpp',
  'src/kline_scheduler.cpp',
]

executable('kline-service',
  srcs,
  include_directories: inc,
  dependencies: [boost_dep, spdlog_dep])
