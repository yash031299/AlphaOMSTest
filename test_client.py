import grpc
import sys
sys.path.insert(0, './proto')
import order_pb2
import order_pb2_grpc

def run():
    channel = grpc.insecure_channel('localhost:50051')
    stub = order_pb2_grpc.OrderServiceStub(channel)

    request = order_pb2.OrderRequest(
        user_id="user123",
        symbol="BTCUSD",
        price=30000.0,
        quantity=0.5,
        side="BUY",
        ordertype="SL",
        ltp=30010.0,
        stopprice=29990.0
    )

    response = stub.SubmitOrder(request)
    print("Response:", response.status, response.message)

if __name__ == '__main__':
    run()
