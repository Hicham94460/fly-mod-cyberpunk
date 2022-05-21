public class FlightSettings {
  public let idk: Float;
  public func airResistance() -> Float = 0.005
  public func angularBrakeFactor() -> Float = -10.0
  public func angularDampFactor() -> Float = -100.0
  public func brakeFactor() -> Float = 1.2
  public func brakeOffset() -> Float = 0.0
  public func collisionRecoveryDelay() -> Float = 0.8
  public func collisionRecoveryDuration() -> Float = 0.8
  public func defaultHoverHeight() -> Float = 3.50
  public func distance() -> Float = 0.0
  public func distanceEase() -> Float = 0.1
  public func fwtfCorrection() -> Float = 0.0
  public func hoverClamp() -> Float = 10.0
  public func hoverFactor() -> Float = 8.0
  public func liftFactor() -> Float = 8.0
  public func liftFactorDrone() -> Float = 30.0
  public func lookAheadMax() -> Float = 10.0
  public func lookAheadMin() -> Float = 1.0
  public func lookDown() -> Vector4 = new Vector4(0.0, 0.0, -this.maxHoverHeight() - 10.0, 0.0);
  public func maxHoverHeight() -> Float = 7.0
  public func minHoverHeight() -> Float = 1.0
  public func normal() -> Vector4 = new Vector4(0.0, 0.0, 1.0, 0.0);
  public func normalEase() -> Float = 0.3
  public func pitchCorrectionFactor() -> Float = 3.0
  public func pitchFactorDrone() -> Float = 0.5
  public func pitchWithLift() -> Float = 0.5
  public func pitchWithSurge() -> Float = 0.0
  public func referenceZ() -> Float = 0.0
  public func rollCorrectionFactor() -> Float = 15.0
  public func rollFactorDrone() -> Float = 5.0
  public func rollWithYaw() -> Float = 0.15
  public func secondCounter() -> Float = 0.0
  public func surgeFactor() -> Float = 15.0
  public func surgeOffset() -> Float = 0.5
  public func swayWithYaw() -> Float =   0.5
  public func thrusterFactor() -> Float = 0.05;
  public func yawCorrectionFactor() -> Float = 0.05
  public func yawD() -> Float = 3.0
  public func yawDirectionalityFactor() -> Float = 30.0
  public func yawFactor() -> Float = 20.0
  public func yawFactorDrone() -> Float = 3.0
}