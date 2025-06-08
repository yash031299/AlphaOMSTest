# 📌 TODO — funding-engine

This file tracks pending work to make `funding-engine` scalable and production-ready.

---

## ✅ Phase 1: Core System

- [x] `FundingCalculator`: long/short OI tracking + rate calculation
- [x] `FundingScheduler`: symbol-specific periodic trigger
- [x] `FundingPublisher`: logs to console
- [x] Boost Asio event loop
- [x] Meson build system
- [x] Dockerfile containerization

---

## 🛠️ Phase 2: Real-time Integration

- [ ] Read open interest from RMS via gRPC
- [ ] Redis publish funding updates to `funding-stream`
- [ ] Add funding pull API via gRPC (`GetFundingRate(symbol)`)

---

## 🧵 Phase 3: Thread Safety & Reliability

- [x] Symbol-isolated data structures
- [ ] Graceful shutdown handler
- [ ] Persistent funding history (optional)

---

## 📊 Phase 4: Monitoring

- [ ] Prometheus metrics:
  - Active symbols
  - Average rate per cycle
  - Redis push latency
- [ ] Liveness probe (`/healthz`)

---

## 🧪 Phase 5: Testing

- [ ] Unit tests for:
  - `FundingCalculator::computeFunding()`
- [ ] Integration test with mock RMS or JSON file
- [ ] Load test with 1000 symbols

---

## 🧠 Notes

- This service is stateless and horizontally scalable
- Each symbol runs independently on Boost timers
- Designed to push rates to Redis or gRPC clients
