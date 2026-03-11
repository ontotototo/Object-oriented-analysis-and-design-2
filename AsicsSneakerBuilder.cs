using SneakerBuilderApp.Models;

namespace SneakerBuilderApp.Builders;

public class AsicsSneakerBuilder : ISneakerBuilder
{
    private readonly Sneaker _sneaker = new();

    public void SetModel(string model) => _sneaker.Model = model;
    public void SetPurpose(string purpose) => _sneaker.Purpose = purpose;
    public void BuildSole(string soleType) => _sneaker.SoleType = soleType;
    public void SetUpper(string upper) => _sneaker.UpperMaterial = upper;
    public void SetColor(string color) => _sneaker.Color = color;

    public void AddGel() => _sneaker.HasGel = true;
    public void ReinforceSole() => _sneaker.ReinforcedSole = true;
    public void SetPremiumUpper() => _sneaker.PremiumUpper = true;
    public void SetCollaboration(string brand) => _sneaker.Collaboration = brand;

    public Sneaker GetSneaker() => _sneaker;
}