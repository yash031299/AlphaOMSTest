# Project Structure

AlphaOMS
├── .devcontainer
│   └── devcontainer.json
├── .github
│   ├── issue_template
│   │   └── alpha_oms_tasks.yml
│   └── workflows
│       └── build.yml
├── core
├── k8s
│   ├── accounting-service.yaml
│   ├── configmap.yaml
│   ├── execution-service.yaml
│   ├── order-service.yaml
│   ├── redis-service.yaml
│   ├── redis.yaml
│   ├── risk-service.yaml
│   ├── stream-service.yaml
│   ├── symbol-service.yaml
│   └── user-service.yaml
├── proto
│   ├── accounting.proto
│   ├── config.proto
│   ├── log_event.proto
│   └── order.proto
├── services
│   ├── accounting-service
│   │   ├── builddir
│   │   │   ├── accounting-service.p
│   │   │   │   ├── src_accounting.grpc.pb.cc.o
│   │   │   │   ├── src_accounting.pb.cc.o
│   │   │   │   ├── src_funding_handler.cpp.o
│   │   │   │   ├── src_grpc_interface.cpp.o
│   │   │   │   ├── src_ledger.cpp.o
│   │   │   │   ├── src_main.cpp.o
│   │   │   │   ├── src_metrics_server.cpp.o
│   │   │   │   ├── src_pnl_tracker.cpp.o
│   │   │   │   ├── src_postgres_writer.cpp.o
│   │   │   │   ├── src_redis_listener.cpp.o
│   │   │   │   └── src_settlement_engine.cpp.o
│   │   │   ├── meson-info
│   │   │   │   ├── intro-benchmarks.json
│   │   │   │   ├── intro-buildoptions.json
│   │   │   │   ├── intro-buildsystem_files.json
│   │   │   │   ├── intro-compilers.json
│   │   │   │   ├── intro-dependencies.json
│   │   │   │   ├── intro-install_plan.json
│   │   │   │   ├── intro-installed.json
│   │   │   │   ├── intro-machines.json
│   │   │   │   ├── intro-projectinfo.json
│   │   │   │   ├── intro-targets.json
│   │   │   │   ├── intro-tests.json
│   │   │   │   └── meson-info.json
│   │   │   ├── meson-logs
│   │   │   │   └── meson-log.txt
│   │   │   ├── meson-private
│   │   │   │   ├── build.dat
│   │   │   │   ├── cmd_line.txt
│   │   │   │   ├── coredata.dat
│   │   │   │   ├── install.dat
│   │   │   │   ├── meson_benchmark_setup.dat
│   │   │   │   ├── meson_test_setup.dat
│   │   │   │   ├── meson.lock
│   │   │   │   ├── sanitycheckcpp.cc
│   │   │   │   └── sanitycheckcpp.exe
│   │   │   ├── .DS_Store
│   │   │   ├── .gitignore
│   │   │   ├── .hgignore
│   │   │   ├── .ninja_deps
│   │   │   ├── .ninja_log
│   │   │   ├── accounting-service
│   │   │   ├── build.ninja
│   │   │   ├── compile_commands.json
│   │   │   ├── test_funding_handler
│   │   │   └── test_ledger
│   │   ├── include
│   │   │   ├── accounting.grpc.pb.h
│   │   │   ├── accounting.pb.h
│   │   │   ├── alert_utils.hpp
│   │   │   ├── funding_handler.hpp
│   │   │   ├── grpc_interface.hpp
│   │   │   ├── ledger.hpp
│   │   │   ├── metrics_server.hpp
│   │   │   ├── pnl_tracker.hpp
│   │   │   ├── postgres_writer.hpp
│   │   │   ├── redis_listener.hpp
│   │   │   └── settlement_engine.hpp
│   │   ├── src
│   │   │   ├── .DS_Store
│   │   │   ├── accounting.grpc.pb.cc
│   │   │   ├── accounting.pb.cc
│   │   │   ├── funding_handler.cpp
│   │   │   ├── grpc_interface.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── ledger.cpp
│   │   │   ├── main.cpp
│   │   │   ├── metrics_server.cpp
│   │   │   ├── pnl_tracker.cpp
│   │   │   ├── postgres_writer.cpp
│   │   │   ├── redis_listener.cpp
│   │   │   └── settlement_engine.cpp
│   │   ├── tests
│   │   │   ├── test_funding_handler.cpp
│   │   │   └── test_ledger.cpp
│   │   ├── .DS_Store
│   │   ├── .env
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── alert-service
│   │   ├── config
│   │   │   └── alert_rules.json
│   │   ├── include
│   │   │   ├── alert_audit_logger.hpp
│   │   │   ├── alert_dispatcher.hpp
│   │   │   ├── alert.hpp
│   │   │   ├── anomaly_detector.hpp
│   │   │   ├── config_loader.hpp
│   │   │   ├── health_check.hpp
│   │   │   ├── metrics.hpp
│   │   │   ├── redis_alert_listener.hpp
│   │   │   ├── redis_anomaly_listener.hpp
│   │   │   ├── redis_publisher.hpp
│   │   │   ├── rule_engine.hpp
│   │   │   └── user_context_provider.hpp
│   │   ├── logs
│   │   │   └── alert.log
│   │   ├── src
│   │   │   ├── alert_audit_logger.cpp
│   │   │   ├── alert_dispatcher.cpp
│   │   │   ├── anomaly_detector.cpp
│   │   │   ├── config_loader.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── main.cpp
│   │   │   ├── metrics_server.cpp
│   │   │   ├── redis_alert_listener.cpp
│   │   │   ├── redis_anomaly_listener.cpp
│   │   │   ├── redis_publisher.cpp
│   │   │   ├── redis_user_context_provider.cpp
│   │   │   └── rule_engine.cpp
│   │   ├── tests
│   │   │   └── rule_engine_test.cpp
│   │   ├── .env
│   │   ├── alerts.db
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── audit-log-service
│   │   ├── data
│   │   │   ├── logs
│   │   │   │   ├── funding
│   │   │   │   ├── liquidation
│   │   │   │   ├── order
│   │   │   │   │   └── 2025-06-11.log
│   │   │   │   ├── trade
│   │   │   │   │   └── 2025-06-11.log
│   │   │   │   ├── .DS_Store
│   │   │   │   └── .gitkeep
│   │   │   ├── logs_test
│   │   │   │   ├── funding
│   │   │   │   ├── liquidation
│   │   │   │   ├── order
│   │   │   │   └── trade
│   │   │   └── .DS_Store
│   │   ├── include
│   │   │   ├── api_server.hpp
│   │   │   ├── grpc_server.hpp
│   │   │   ├── health_check.hpp
│   │   │   ├── log_event.grpc.pb.h
│   │   │   ├── log_event.pb.h
│   │   │   ├── log_writer_async.hpp
│   │   │   ├── log_writer.hpp
│   │   │   └── redis_consumer.hpp
│   │   ├── src
│   │   │   ├── api_server.cpp
│   │   │   ├── grpc_server.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── log_event.grpc.pb.cc
│   │   │   ├── log_event.pb.cc
│   │   │   ├── log_writer_async.cpp
│   │   │   ├── log_writer.cpp
│   │   │   ├── main.cpp
│   │   │   └── redis_consumer.cpp
│   │   ├── tests
│   │   │   └── test_log_writer.cpp
│   │   ├── .DS_Store
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── config-service
│   │   ├── include
│   │   │   ├── config_loader.hpp
│   │   │   └── grpc_server.hpp
│   │   ├── src
│   │   │   ├── config_loader.cpp
│   │   │   ├── grpc_server.cpp
│   │   │   ├── health_checker.cpp
│   │   │   └── main.cpp
│   │   └── meson.build
│   ├── execution-service
│   │   ├── include
│   │   │   ├── execution_router.hpp
│   │   │   ├── executor_limit.hpp
│   │   │   ├── executor_market.hpp
│   │   │   ├── executor_sl.hpp
│   │   │   ├── grpc_server.hpp
│   │   │   ├── logger.hpp
│   │   │   ├── order.grpc.pb.h
│   │   │   ├── order.pb.h
│   │   │   ├── pnl_client.hpp
│   │   │   ├── redis_publisher.hpp
│   │   │   └── user_client.hpp
│   │   ├── src
│   │   │   ├── execution_router.cpp
│   │   │   ├── executor_limit.cpp
│   │   │   ├── executor_market.cpp
│   │   │   ├── executor_sl.cpp
│   │   │   ├── grpc_server.cpp
│   │   │   ├── health_checker.cpp
│   │   │   ├── logger.cpp
│   │   │   ├── main.cpp
│   │   │   ├── order.grpc.pb.cc
│   │   │   ├── order.pb.cc
│   │   │   ├── pnl_client.cpp
│   │   │   ├── redis_publisher.cpp
│   │   │   └── user_client.cpp
│   │   ├── .DS_Store
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO_EXECUTION.md
│   ├── funding-engine
│   │   ├── include
│   │   │   ├── funding_calculator.hpp
│   │   │   ├── funding_publisher.hpp
│   │   │   └── funding_scheduler.hpp
│   │   ├── src
│   │   │   ├── funding_calculator.cpp
│   │   │   ├── funding_publisher.cpp
│   │   │   ├── funding_scheduler.cpp
│   │   │   ├── health_check.cpp
│   │   │   └── main.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── kline-service
│   │   ├── include
│   │   │   ├── candle_aggregator.hpp
│   │   │   ├── candle_types.hpp
│   │   │   └── kline_scheduler.hpp
│   │   ├── src
│   │   │   ├── candle_aggregator.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── kline_scheduler.cpp
│   │   │   └── main.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── liquidation-service
│   │   ├── include
│   │   │   └── liquidation_service.hpp
│   │   ├── src
│   │   │   └── liquidation_service.cpp
│   │   ├── Dockerfile
│   │   ├── main.cpp
│   │   └── meson.build
│   ├── match-engine
│   │   ├── config
│   │   │   └── symbols.json
│   │   ├── include
│   │   │   ├── book.hpp
│   │   │   ├── matcher.hpp
│   │   │   └── trade_publisher.hpp
│   │   ├── src
│   │   │   ├── book.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── main.cpp
│   │   │   ├── matcher.cpp
│   │   │   └── trade_publisher.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── order-router-service
│   │   ├── include
│   │   │   ├── grpc_client.hpp
│   │   │   └── order_router.hpp
│   │   ├── src
│   │   │   ├── grpc_client.cpp
│   │   │   └── order_router.cpp
│   │   ├── Dockerfile
│   │   ├── main.cpp
│   │   └── meson.build
│   ├── order-service
│   │   ├── include
│   │   │   ├── controller.hpp
│   │   │   ├── grpc_client.hpp
│   │   │   ├── router.hpp
│   │   │   ├── symbol_client.hpp
│   │   │   ├── user_client.hpp
│   │   │   └── validator.hpp
│   │   ├── src
│   │   │   ├── controller.cpp
│   │   │   ├── grpc_client.cpp
│   │   │   ├── health_checker.cpp
│   │   │   ├── main.cpp
│   │   │   ├── router.cpp
│   │   │   ├── symbol_client.cpp
│   │   │   ├── user_client.cpp
│   │   │   └── validator.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO_ORDER.md
│   ├── pnl-service
│   │   ├── config
│   │   │   └── test_ticks.json
│   │   ├── include
│   │   │   ├── equity_calculator.hpp
│   │   │   ├── grpc_interface.hpp
│   │   │   └── pnl_engine.hpp
│   │   ├── proto
│   │   │   └── pnl.proto
│   │   ├── src
│   │   │   ├── equity_calculator.cpp
│   │   │   ├── grpc_interface.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── main.cpp
│   │   │   └── pnl_engine.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── redis-service
│   │   ├── include
│   │   │   ├── order_updater.hpp
│   │   │   ├── redis_connection.hpp
│   │   │   └── trade_broadcaster.hpp
│   │   ├── src
│   │   │   ├── health_checker.cpp
│   │   │   ├── main.cpp
│   │   │   ├── order_updater.cpp
│   │   │   ├── redis_connection.cpp
│   │   │   └── trade_broadcaster.cpp
│   │   ├── Dockerfile
│   │   └── meson.build
│   ├── report-export-service
│   │   ├── include
│   │   │   ├── csv_writer.hpp
│   │   │   ├── pdf_writer.hpp
│   │   │   └── report_generator.hpp
│   │   ├── src
│   │   │   ├── csv_writer.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── main.cpp
│   │   │   ├── pdf_writer.cpp
│   │   │   └── report_generator.cpp
│   │   ├── templates
│   │   │   └── statement_template.html
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── risk-service
│   │   ├── include
│   │   │   ├── liquidation_manager.hpp
│   │   │   ├── ltp_listener.hpp
│   │   │   ├── margin_listener.hpp
│   │   │   ├── metrics.hpp
│   │   │   ├── position_tracker.hpp
│   │   │   ├── redis_connection.hpp
│   │   │   ├── redis_retry.hpp
│   │   │   ├── risk_evaluator.hpp
│   │   │   ├── rms_engine.hpp
│   │   │   └── user_state.hpp
│   │   ├── src
│   │   │   ├── health_checker.cpp
│   │   │   ├── liquidation_manager.cpp
│   │   │   ├── ltp_listener.cpp
│   │   │   ├── main.cpp
│   │   │   ├── margin_listener.cpp
│   │   │   ├── position_tracker.cpp
│   │   │   ├── redis_connection.cpp
│   │   │   ├── risk_evaluator.cpp
│   │   │   └── rms_engine.cpp
│   │   ├── tests
│   │   │   └── test_rms.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO_RMS.md
│   ├── stream-service
│   │   ├── include
│   │   │   ├── redis_subscriber.hpp
│   │   │   └── tick_parser.hpp
│   │   ├── src
│   │   │   ├── health_checker.cpp
│   │   │   ├── main.cpp
│   │   │   ├── redis_subscriber.cpp
│   │   │   └── tick_parser.cpp
│   │   ├── Dockerfile
│   │   └── meson.build
│   ├── symbol-registry-service
│   │   ├── client
│   │   │   └── test_symbol_client.py
│   │   ├── config
│   │   │   └── symbols.json
│   │   ├── include
│   │   │   ├── symbol_service.hpp
│   │   │   ├── symbol.grpc.pb.h
│   │   │   └── symbol.pb.h
│   │   ├── proto
│   │   │   └── symbol.proto
│   │   ├── src
│   │   │   ├── grpc_server.cpp
│   │   │   ├── main.cpp
│   │   │   ├── symbol_service.cpp
│   │   │   ├── symbol.grpc.pb.cc
│   │   │   └── symbol.pb.cc
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO_SYMBOL.md
│   ├── symbol-service
│   │   ├── include
│   │   │   ├── orderbook_manager.hpp
│   │   │   └── snapshot_generator.hpp
│   │   ├── src
│   │   │   ├── health_check.cpp
│   │   │   ├── main.cpp
│   │   │   ├── orderbook_manager.cpp
│   │   │   └── snapshot_generator.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO_SYMBOL.md
│   ├── user-service
│   │   ├── include
│   │   │   ├── grpc_user_server.hpp
│   │   │   ├── margin_listener.hpp
│   │   │   ├── redis_safe_client.hpp
│   │   │   ├── scheduler.hpp
│   │   │   ├── user_metadata_store.hpp
│   │   │   └── user_wallet_store.hpp
│   │   ├── src
│   │   │   ├── health_checker.cpp
│   │   │   ├── main.cpp
│   │   │   ├── margin_listener.cpp
│   │   │   ├── redis_safe_client.cpp
│   │   │   ├── scheduler.cpp
│   │   │   ├── user_metadata_store.cpp
│   │   │   └── user_wallet_store.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO_USER.md
│   ├── wallet-sync-service
│   │   ├── include
│   │   │   ├── redis_listener.hpp
│   │   │   └── wallet_store.hpp
│   │   ├── src
│   │   │   ├── health_checker.cpp
│   │   │   ├── main.cpp
│   │   │   ├── redis_listener.cpp
│   │   │   └── wallet_store.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   └── .DS_Store
├── shared
│   └── json
├── src
├── tools
│   └── test_order_cli.cpp
├── .DS_Store
├── .env
├── .gitignore
├── conanfile.txt
├── docker-compose.yml
├── get-pip.py
├── meson.build
├── README.md
├── structure.md
└── tasks.md
