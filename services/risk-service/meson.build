executable('risk_service',
  ['src/main.cpp', 'src/ltp_listener.cpp', 'src/risk_evaluator.cpp', 'src/liquidation_manager.cpp', 'src/position_tracker.cpp'],
  include_directories : include_directories('include'),
  dependencies : [
    dependency('fmt'),
    dependency('spdlog'),
    dependency('boost'),
    dependency('hiredis'),
    dependency('rapidjson'),
    dependency('crow', required: true)
  ]
)
test_exe = executable('test_rms', 'tests/test_rms.cpp',
  include_directories: inc,
  dependencies: [doctest_dep]
  
)
test('rms unit tests', test_exe)