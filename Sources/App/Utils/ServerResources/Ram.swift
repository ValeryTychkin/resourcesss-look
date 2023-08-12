class ServerRam {
    let socketClient: SocketClient
    
    // TODO add swap
    
    var usageRamBits: Int {
        // Return usage ram bits on server
        return self.getUsageRam()
    }
    var usageRamPrecent: Float {
        // Return usage precent of ram on server
        return (
            (Float(self.usageRamBits) * 100.0) / Float(self.totalRamBits)
        )
    }
    var totalRamBits: Int {
        // Return total ram bits on server
        return self.getTotalRam()
    }
    
    private func getTotalRam() -> Int {
        // Return total ram bits on server
        return 0
    }
    
    private func getUsageRam() -> Int {
        // Return usage ram bits on server
        return 0
    }
    
    init(socketClient: SocketClient) {
        self.socketClient = socketClient
    }
}
