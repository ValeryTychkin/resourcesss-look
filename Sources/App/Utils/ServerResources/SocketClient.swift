import Foundation
import CClientSocket

class SocketClient {
    private func jsontToDictionary(str: String) -> [String: Any] {
        if let data = str.data(using: .utf8) {
            do {
                return try JSONSerialization.jsonObject(with: data, options: []) as! [String: Any]
            } catch {
                print(error.localizedDescription)
            }
        }
        return ["err": "Failed to convert: \(str)"]
    }
    
    func cRunFunction() -> String {
        let buffer: UnsafeMutablePointer<CChar> = UnsafeMutablePointer<CChar>.allocate(capacity: Int(BUFFER_SIZE))
        cGetRumMemoryInfo(buffer)
        return String(cString: buffer)
    }
    
    func getRamMemoryInfo() -> [String: Any] {
        return ["kek": "keeeek"]
    }
}
