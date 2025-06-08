# 📌 TODO — kline-service

This service aggregates trades into OHLCV candles per symbol and interval.

---

## ✅ Phase 1: Core System

- [x] TradeTick and Candle structs (1m, 5m, 1h)
- [x] In-memory CandleAggregator
- [x] KlineScheduler with flush timer
- [x] Boost Asio + thread-safe loop
- [x] Meson build and Dockerfile

---

## 🔁 Phase 2: Input/Output Integration

- [ ] Subscribe to trade feed from match-engine (via Redis, gRPC, or stream)
- [ ] Push candles to:
  - Redis Pub/Sub channel `ohlcv-stream`
  - File log / audit-log-service
  - PostgreSQL/ClickHouse (optional)

---

## 🔒 Phase 3: Stability and Scaling

- [ ] Cap candle history (optional in-memory LRU cache)
- [ ] Batched DB insert with retries
- [ ] Memory profiler on flush cycles
- [ ] Deduplicate ticks per timestamp if source is unreliable

---

## 📊 Phase 4: Observability

- [ ] Prometheus metrics:
  - `kline_flush_duration`
  - `kline_ohlcv_count`
- [ ] Health check ping `/healthz`

---

## 🧪 Phase 5: Testing

- [ ] Unit test `addTrade()` aggregation logic
- [ ] Integration test flush cycle timing
- [ ] Load test with 10k trades/sec on 1000 symbols

---

## 🧠 Notes

- Horizontally scalable by sharding symbols across instances
- Purely in-memory — persist via output pipeline
- Extensible to 1s candles, daily, etc.
