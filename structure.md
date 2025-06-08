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
│   └── order.proto
├── services
│   ├── accounting-service
│   │   ├── include
│   │   │   ├── funding_handler.hpp
│   │   │   ├── ledger.hpp
│   │   │   ├── pnl_tracker.hpp
│   │   │   └── settlement_engine.hpp
│   │   ├── src
│   │   │   ├── funding_handler.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── ledger.cpp
│   │   │   ├── main.cpp
│   │   │   ├── pnl_tracker.cpp
│   │   │   └── settlement_engine.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── alert-service
│   │   ├── config
│   │   │   └── alert_rules.json
│   │   ├── include
│   │   │   ├── alert_dispatcher.hpp
│   │   │   ├── anomaly_detector.hpp
│   │   │   └── rule_engine.hpp
│   │   ├── src
│   │   │   ├── alert_dispatcher.cpp
│   │   │   ├── anomaly_detector.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── main.cpp
│   │   │   └── rule_engine.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── audit-log-service
│   │   ├── data
│   │   │   └── logs
│   │   │       └── .gitkeep
│   │   ├── include
│   │   │   ├── api_server.hpp
│   │   │   ├── event_types.hpp
│   │   │   └── log_writer.hpp
│   │   ├── src
│   │   │   ├── api_server.cpp
│   │   │   ├── health_check.cpp
│   │   │   ├── log_writer.cpp
│   │   │   └── main.cpp
│   │   ├── Dockerfile
│   │   ├── meson.build
│   │   └── TODO.md
│   ├── config-service
│   │   ├── include
│   │   │   └── config_loader.hpp
│   │   ├── src
│   │   │   ├── config_loader.cpp
│   │   │   └── health_checker.cpp
│   │   └── meson.build
│   ├── execution-service
│   │   ├── include
│   │   │   ├── executor_limit.hpp
│   │   │   ├── executor_market.hpp
│   │   │   ├── executor_sl.hpp
│   │   │   ├── logger.hpp
│   │   │   ├── order.grpc.pb.h
│   │   │   └── order.pb.h
│   │   ├── src
│   │   │   ├── executor_limit.cpp
│   │   │   ├── executor_market.cpp
│   │   │   ├── executor_sl.cpp
│   │   │   ├── grpc_server.cpp
│   │   │   ├── health_checker.cpp
│   │   │   ├── logger.cpp
│   │   │   ├── main.cpp
│   │   │   ├── order.grpc.pb.cc
│   │   │   └── order.pb.cc
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
│   ├── order-service
│   │   ├── include
│   │   │   ├── controller.hpp
│   │   │   ├── grpc_client.hpp
│   │   │   ├── router.hpp
│   │   │   └── validator.hpp
│   │   ├── src
│   │   │   ├── controller.cpp
│   │   │   ├── grpc_client.cpp
│   │   │   ├── health_checker.cpp
│   │   │   ├── main.cpp
│   │   │   ├── router.cpp
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
│   │   │   ├── metrics.hpp
│   │   │   ├── position_tracker.hpp
│   │   │   ├── redis_retry.hpp
│   │   │   ├── risk_evaluator.hpp
│   │   │   ├── rms_engine.hpp
│   │   │   └── user_state.hpp
│   │   ├── src
│   │   │   ├── health_checker.cpp
│   │   │   ├── liquidation_manager.cpp
│   │   │   ├── ltp_listener.cpp
│   │   │   ├── main.cpp
│   │   │   ├── position_tracker.cpp
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
│   └── .DS_Store
├── shared
│   └── json
├── tools
│   └── test_order_cli.cpp
├── .DS_Store
├── .env
├── .gitignore
├── conanfile.txt
├── docker-compose.yml
├── meson.build
├── README.md
└── tasks.md
