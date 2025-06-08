executable('wallet-sync-service',
  'src/main.cpp',
  'src/redis_listener.cpp',
  'src/wallet_store.cpp',
  'src/health_checker.cpp',
  include_directories: include_directories('include'),
  dependencies: [
    dependency('crow', required: true),
    dependency('nlohmann_json', required: true),
    dependency('hiredis', required: true),
    dependency('spdlog', required: true)
  ]
)
