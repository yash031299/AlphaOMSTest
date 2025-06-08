# ✅ Alpha OMS — Microservice Build Tracker

## 🔁 Status Legend

- [x] Done
- [ ] Pending
- [~] In progress / Partially complete

---

## 🔨 Core Services

### 1. Order Service

- [x] Controller
- [x] Validator
- [x] Router
- [x] gRPC client to Execution Service
- [x] main.cpp integration

### 2. Execution Service

- [x] Executor: Limit
- [x] Executor: Market
- [x] Executor: SL
- [x] gRPC Server
- [x] gRPC Proto Handling
- [x] grpc_server.cpp
- [x] main.cpp updated

### 3. Risk Service

- [x] LTP Listener
- [x] Risk Evaluator
- [x] Liquidation Manager
- [x] Position Tracker
- [x] main.cpp integration

### 4. User Service

- [x] Wallet Manager
- [x] Balance Evaluator
- [x] Scheduler (loop thread)
- [x] main.cpp

### 5. Symbol Service

- [x] Orderbook Manager
- [x] Snapshot Generator
- [x] main.cpp

### 6. Stream Service

- [x] Redis Subscriber
- [x] Tick Parser
- [x] main.cpp

### 7. Redis Service

- [x] Order Updater
- [x] Trade Broadcaster
- [x] Redis Connection
- [x] main.cpp

### 8. Config Service

- [x] Config Loader (.env)
- [x] Shared library setup
- [x] Usage in services

---

## 🧪 Testing & CLI

- [x] CLI generator (`test_order_cli.cpp`)
- [x] STDIN support in order-service
- [ ] Fix CLI build (nlohmann/json + iostream issue)

---

## 🐳 Dockerization

- [x] order-service Dockerfile
- [ ] All other Dockerfiles
- [x] docker-compose.yml
- [ ] Healthchecks
- [ ] Log volume config (optional)

---

## 📦 Build System

- [x] Conan setup
- [x] Meson build files per service
- [x] Protobuf + gRPC integration
- [x] Shared include and src directories
- [x] .gitignore

---

## 📄 Documentation

- [x] README.md
- [x] Folder structure
- [x] Meson + Conan instructions
- [x] gRPC & Redis notes
- [ ] Design Patterns used (SOLID explanation)

---

## 💬 Post-Build Phase

- [ ] C# gRPC client integration
- [ ] React UI (frontend) integration
- [ ] End-to-end test cases
- [ ] CI/CD pipelines
- [ ] Deployment (Kubernetes / Docker Swarm)
