import grpc
import client.symbol_pb2 as symbol_pb2
import client.symbol_pb2_grpc as symbol_pb2_grpc

def test_symbol(symbol):
    channel = grpc.insecure_channel("localhost:50053")
    stub = symbol_pb2_grpc.SymbolServiceStub(channel)
    response = stub.GetSymbolInfo(symbol_pb2.SymbolRequest(symbol=symbol))
    print(f"✅ Symbol Info for {symbol}:")
    print("  Tick Size:", response.tick_size)
    print("  Precision:", response.price_precision)
    print("  Max Leverage:", response.max_leverage)
    print("  Base:", response.base_currency, "| Quote:", response.quote_currency)

if __name__ == "__main__":
    test_symbol("BTCUSDT")
