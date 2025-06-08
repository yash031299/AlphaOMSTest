project('report-export-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

boost_dep = dependency('boost', required: false)
spdlog_dep = dependency('spdlog')
nlohmann_json_dep = dependency('nlohmann_json')

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/report_generator.cpp',
  'src/csv_writer.cpp',
  'src/pdf_writer.cpp'
]

executable('report-export-service',
  srcs,
  include_directories: inc,
  dependencies: [spdlog_dep, nlohmann_json_dep])
