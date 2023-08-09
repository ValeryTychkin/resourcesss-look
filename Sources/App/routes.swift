import Fluent
import Vapor

func routes(_ app: Application) throws {
    app.get("health-check") { req async in
        "OK"
    }
}
