# 📌 TODO — rms-service (Risk Management Engine)

This file tracks all technical and architectural improvements required to make RMS production-ready, scalable, and resilient.

---

## ✅ Phase 1: Core Fixes and Technical Debt

- [ ] Replace per-user `steady_timer` model with a **batched timer** + fan-out loop
- [ ] Pool or reuse `redisContext` connections (avoid reconnecting on every tick)
- [ ] Optimize `onPriceUpdate()` to avoid redundant state updates
- [ ] Cache last LTP to prevent re-processing same price repeatedly
- [ ] Move Redis parsing logic to a clean utility (decouple from logic)

---

## 🛡️ Phase 2: Fault Tolerance & Reliability

- [ ] Implement Redis reconnect logic with backoff (in case of Redis downtime)
- [ ] Add circuit-breaker behavior: if Redis fails continuously, pause RMS updates
- [ ] Add config fallback: default margin ratios if LTP is unavailable
- [ ] Add `std::optional` or error wrappers to Redis commands to handle failures gracefully

---

## 📊 Phase 3: Monitoring and Metrics

- [ ] Add Prometheus metrics:
  - Gauge: `rms_active_users`
  - Counter: `rms_liquidations_total`
  - Histogram: `rms_onPriceUpdate_duration_seconds`
  - Gauge: `rms_user_equity` (per user or aggregated)
- [ ] Expose `/metrics` endpoint using Prometheus C++ client
- [ ] Add alerts (Redis disconnected, high liquidation rate, missed updates)

---

## 🧵 Phase 4: Concurrency and Performance

- [ ] Replace `std::unordered_map<std::string, shared_ptr<RMSEngine>>` with a **sharded concurrent map**
- [ ] Use thread pool (Boost.Asio `thread_pool` or custom) to process `onPriceUpdate` in parallel
- [ ] Eliminate duplicate margin syncs for the same user
- [ ] Benchmark max supported users and optimize accordingly

---

## 🔬 Phase 5: Testing and Validation

- [ ] Add unit tests for `evaluateRisk()`, `syncMargin()`, and `liquidate()`
- [ ] Add fuzz tests for malformed Redis tick data
- [ ] Add load test to simulate 1,000 users receiving LTP updates every second
- [ ] Validate formulas with real trading scenarios (from attached gists)

---

## 💡 Phase 6: Engineering Cleanliness

- [ ] Move `user_state`, `rms_engine`, and `RMSMetrics` into submodules or dedicated headers
- [ ] Load config (e.g., margin ratio, price feed name) from `.env` or `config.json`
- [ ] Apply full SOLID principles:
  - Single Responsibility: break large `rms_engine.cpp` into strategy, sync, and risk units
  - Dependency Inversion: abstract Redis pub/sub into interface

---

## Summary

| Area                   | Status              |
| ---------------------- | ------------------- |
| Core RMS logic         | ✅ Functional       |
| Scaling to 10k users   | ⚠️ Needs batching   |
| Fault handling         | ❌ Missing          |
| Monitoring/metrics     | ❌ Missing          |
| Unit/integration tests | ❌ Missing          |
| Clean code principles  | ⚠️ Needs modularity |
