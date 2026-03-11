using SneakerBuilderApp.Builders;
using SneakerBuilderApp.Models;
namespace SneakerBuilderApp.Directors;

public class SneakerDirector
{
    public void BuildBaseSneaker(
        ISneakerBuilder builder,
        SneakerType type)
    {
        switch (type)
        {
            case SneakerType.Volleyball:
                builder.SetModel("ASICS Volleyball Pro");
                builder.SetPurpose("Волейбол");
                builder.BuildSole("Резиновая");
                builder.SetUpper("Спортивная сетка");
                builder.SetColor("Белый/Синий");
                break;

            case SneakerType.Urban:
                builder.SetModel("ASICS Urban Walk");
                builder.SetPurpose("Город");
                builder.BuildSole("Лёгкая");
                builder.SetUpper("Текстиль");
                builder.SetColor("Чёрный");
                break;

            case SneakerType.Collaboration:
                builder.SetModel("ASICS Limited");
                builder.SetPurpose("Коллекционная");
                builder.BuildSole("Премиальная");
                builder.SetUpper("Натуральная кожа");
                builder.SetColor("Фирменный");
                break;
        }
    }
}