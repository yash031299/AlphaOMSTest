# 📌 TODO — symbol-registry-service

Tracks planned enhancements, optimizations, and production-readiness goals.

---

## ✅ Phase 1: Core Functionality

- [x] Load `symbols.json` at startup
- [x] Serve metadata via gRPC
- [x] Implement `GetSymbolMetadata` with locking
- [x] Validate tick size, leverage, precision
- [x] Containerize service with Meson + Docker

---

## 🔄 Phase 2: Extensibility

- [ ] Add gRPC endpoint: `ListAllSymbols()`
- [ ] Support `/reload` for hot config reload (file watcher or API trigger)
- [ ] Load from Redis (optional for dynamic symbols)

---

## 🔒 Phase 3: Security & Reliability

- [ ] Use TLS for gRPC transport
- [ ] Add authentication (token / cert-based)
- [ ] Input validation: enforce lowercase symbols, safe limits

---

## 📊 Phase 4: Observability

- [ ] Add Prometheus metrics:
  - Total RPCs served
  - Failed lookups
  - Hot reloads triggered
- [ ] Add `/healthz` HTTP probe

---

## 🧪 Phase 5: Testing

- [ ] Unit tests for:
  - `loadSymbolsFromFile()`
  - `GetSymbolMetadata()`
- [ ] Add gRPC integration tests using Python client
- [ ] Fuzz test for malformed requests

---

## 🚀 Deployment Readiness

- [ ] Add `k8s/service.yaml` and `deployment.yaml`
- [ ] Preload with 1000+ symbols and benchmark gRPC latency
- [ ] Expose as cluster-internal endpoint (used by RMS, order-service)
