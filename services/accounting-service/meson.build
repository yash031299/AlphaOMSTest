project('accounting-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

boost_dep = dependency('boost', modules: ['asio'])
spdlog_dep = dependency('spdlog')

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/ledger.cpp',
  'src/funding_handler.cpp',
  'src/pnl_tracker.cpp',
  'src/settlement_engine.cpp'
]

executable('accounting-service',
  srcs,
  include_directories: inc,
  dependencies: [boost_dep, spdlog_dep])
