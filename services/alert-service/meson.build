project('alert-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

# Dependency: Boost (Asio, System, Thread)
boost_inc = include_directories('/opt/homebrew/opt/boost/include')
boost_lib = declare_dependency(
  include_directories: boost_inc,
  link_args: ['-L/opt/homebrew/opt/boost/lib', '-lboost_system', '-lboost_thread']
)

# Dependency: spdlog
spdlog_dep = dependency('spdlog', required: true)

# Dependency: hiredis
hiredis_dep = declare_dependency(
  include_directories: include_directories('/opt/homebrew/include'),
  link_args: ['-L/opt/homebrew/lib', '-lhiredis']
)

# Dependency: nlohmann-json (header-only)
json_dep = declare_dependency(
  include_directories: include_directories('/opt/homebrew/include')
)

# Dependency: sqlite3
sqlite_dep = dependency('sqlite3', required: true)

# Include headers
inc = include_directories('include')

# Source files
srcs = [
  'src/main.cpp',
  'src/rule_engine.cpp',
  'src/anomaly_detector.cpp',
  'src/alert_dispatcher.cpp',
  'src/redis_alert_listener.cpp',
  'src/redis_anomaly_listener.cpp',
  'src/redis_publisher.cpp',
  'src/metrics_server.cpp',
  'src/health_check.cpp',
  'src/config_loader.cpp',
  'src/alert_audit_logger.cpp',
  'src/redis_user_context_provider.cpp'
]

add_project_arguments('-w', language: 'cpp')  # suppress all warnings

# Final build target
executable('alert-service',
  srcs,
  include_directories: inc,
  dependencies: [
    boost_lib,
    spdlog_dep,
    hiredis_dep,
    json_dep,
    sqlite_dep
  ]
)

# Optional test
test('rule_engine_test',
  executable('rule_engine_test',
    'tests/rule_engine_test.cpp',
    'src/rule_engine.cpp',
    include_directories: inc,
    dependencies: [spdlog_dep, json_dep]
  )
)
