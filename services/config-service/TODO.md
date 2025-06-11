# 🧩 config-service — TODO Checklist

Tracks progress of the microservice that loads and serves config data via gRPC.

---

## ✅ CORE FUNCTIONALITY

- [x] Load `.env` file for dynamic config path and port
- [x] Load symbol configuration JSON from file
- [x] Expose `GetConfig` RPC for a single symbol
- [x] Expose `GetAll` RPC for dumping all ENV keys
- [x] Catch and log all exceptions (crash-proof)

---

## 🧪 TESTING & RESILIENCE

- [x] Validate JSON structure on startup
- [x] Gracefully handle missing symbol keys
- [x] Handle file not found / parse errors
- [ ] Unit tests (deferred)
- [ ] Integration test with RMS or order-service (later)

---

## 🏗️ BUILD & DEPLOYMENT

- [ ] Complete `meson.build` with all source files
- [ ] Integrate gRPC + protobuf compiler steps
- [ ] Add Conan dependency for `nlohmann_json`
- [ ] Create Dockerfile for containerization
- [ ] Add `health_checker` to Docker container

---

## 💡 FUTURE ENHANCEMENTS

- [ ] Support YAML config file (fallback to JSON)
- [ ] Add live config reload (watcher or admin trigger)
- [ ] Validate config schema before serving
- [ ] Add `/metrics` endpoint for Prometheus

---

_Progress last updated: {{DATE}}_
