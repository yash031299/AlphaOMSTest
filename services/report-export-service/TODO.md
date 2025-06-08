# 📌 TODO — report-export-service

This service generates CSV and PDF exports from structured JSON data.

---

## ✅ Phase 1: Core Exporters

- [x] CSV export
- [x] PDF export via HTML and wkhtmltopdf
- [x] Local output to `data/exports/`

---

## 🔄 Phase 2: Integration

- [ ] Accept gRPC/Redis request for report generation
- [ ] Support export types: ledger, pnl, trade history
- [ ] Stream response (download URL or direct)

---

## 🛡️ Phase 3: Hardening

- [ ] Validate input schema (schema.json)
- [ ] Limit export size per request (max 10k rows)
- [ ] Output filename hash to avoid collisions

---

## 📊 Phase 4: Observability

- [ ] Prometheus metrics: total exports, errors
- [ ] Healthcheck endpoint

---

## 🧪 Phase 5: Testing

- [ ] Unit test CSVWriter/PDFWriter
- [ ] Integration test end-to-end generation
- [ ] Stress test with 10,000-row export

---

## 🧠 Notes

- Requires wkhtmltopdf installed in container
- Modular design: writers are reusable
- Can later integrate with alert-service to auto-export on liquidation
