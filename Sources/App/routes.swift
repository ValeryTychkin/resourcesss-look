import Fluent
import Vapor

struct RamInfoResponse: Content {
    var total: Int = 0
    var usage: Int = 0
    var availble: Int = 0
    var usagePercent: Float = 0
}

func routes(_ app: Application) throws {
    app.get("health-check") { req async in
        return "OK"
    }
    
    app.get("ram-info") { req async -> RamInfoResponse in
        let ramInfo = RamInfoResponse()
        return ramInfo
    }
    
    app.get("test") { req async in
        let result = SocketClient.init()
        return result.cRunFunction()
    }
}
