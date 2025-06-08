project('match-engine', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

spdlog_dep = dependency('spdlog')

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/matcher.cpp',
  'src/book.cpp',
  'src/trade_publisher.cpp',
]

executable('match-engine',
  srcs,
  include_directories: inc,
  dependencies: [spdlog_dep])
