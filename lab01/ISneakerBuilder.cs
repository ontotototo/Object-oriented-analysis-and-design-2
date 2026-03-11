using SneakerBuilderApp.Models; 

namespace SneakerBuilderApp.Builders;

public interface ISneakerBuilder
{
    void SetModel(string model);
    void SetPurpose(string purpose);
    void BuildSole(string soleType);
    void SetUpper(string upper);
    void SetColor(string color);

    // опциональные шаги
    void AddGel();
    void ReinforceSole();
    void SetPremiumUpper();
    void SetCollaboration(string brand);

    Sneaker GetSneaker();
}