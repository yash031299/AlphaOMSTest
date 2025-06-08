project('funding-engine', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

spdlog_dep = dependency('spdlog')
boost_dep = dependency('boost', modules: ['asio'])

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/funding_calculator.cpp',
  'src/funding_scheduler.cpp',
  'src/funding_publisher.cpp',
]

executable('funding-engine',
  srcs,
  include_directories: inc,
  dependencies: [spdlog_dep, boost_dep])
