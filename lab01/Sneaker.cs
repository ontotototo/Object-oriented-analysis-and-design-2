namespace SneakerBuilderApp.Models;

public class Sneaker
{
    public string Model { get; set; } = "";
    public string Purpose { get; set; } = "";
    public string SoleType { get; set; } = "";

    public bool HasGel { get; set; }
    public bool ReinforcedSole { get; set; }
    public bool PremiumUpper { get; set; }

    public string UpperMaterial { get; set; } = "";
    public string Color { get; set; } = "";
    public string Collaboration { get; set; } = "";

    public string GetDescription()
    {
        return
            $"Модель: {Model}\n" +
            $"Назначение: {Purpose}\n" +
            $"Подошва: {SoleType}\n" +
            $"Гель: {(HasGel ? "Да" : "Нет")}\n" +
            $"Усиленная подошва: {(ReinforcedSole ? "Да" : "Нет")}\n" +
            $"Премиальный верх: {(PremiumUpper ? "Да" : "Нет")}\n" +
            (string.IsNullOrEmpty(Collaboration)
                ? ""
                : $"Коллаборация: {Collaboration}\n");
    }
}