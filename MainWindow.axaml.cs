using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Media.Imaging;
using Avalonia.Platform;
using SneakerBuilderApp.Builders;
using SneakerBuilderApp.Directors;
using SneakerBuilderApp.Models;
using System.Threading.Tasks;
using System;

namespace SneakerBuilderApp;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }

    private async void BuildButton_Click(object? sender, RoutedEventArgs e)
    {
        LogBox.Text = "";
        ResultBox.Text = "";
        Progress.Value = 0;

        var builder = new AsicsSneakerBuilder();
        var director = new SneakerDirector();

        SneakerType type =
            VolleyballRB.IsChecked == true ? SneakerType.Volleyball :
            UrbanRB.IsChecked == true ? SneakerType.Urban :
            SneakerType.Collaboration;

        director.BuildBaseSneaker(builder, type);
        Log("Создана базовая модель");
        await AnimateStep();

        if (GelCB.IsChecked == true)
        {
            builder.AddGel();
            Log("Добавлена гелевая амортизация");
            await AnimateStep();
        }

        if (ReinforcedSoleCB.IsChecked == true)
        {
            builder.ReinforceSole();
            Log("Подошва усилена");
            await AnimateStep();
        }

        if (PremiumUpperCB.IsChecked == true)
        {
            builder.SetPremiumUpper();
            Log("Добавлен премиальный верх");
            await AnimateStep();
        }

        var sneaker = builder.GetSneaker();
        ResultBox.Text = sneaker.GetDescription();

        Log("Сборка завершена");
        Progress.Value = 100;

        ShowFinalImage(type, sneaker.PremiumUpper);
    }

    private void Log(string message)
    {
        LogBox.Text += message + "\n";
    }

    private async Task AnimateStep()
    {
        Progress.Value += 20;
        await Task.Delay(350);
    }

    private void ShowFinalImage(SneakerType type, bool premium)
    {
        string path = type switch
        {
            SneakerType.Volleyball when premium =>
                "avares://SneakerBuilderApp/Assets/volleyball_premium.png",

            SneakerType.Volleyball =>
                "avares://SneakerBuilderApp/Assets/volleyball_basic.png",

            SneakerType.Urban when premium =>
                "avares://SneakerBuilderApp/Assets/urban_premium.png",

            SneakerType.Urban =>
                "avares://SneakerBuilderApp/Assets/urban_basic.png",

            _ =>
                "avares://SneakerBuilderApp/Assets/collab.png"
        };

        SneakerImage.Source =
            new Bitmap(AssetLoader.Open(new Uri(path)));
    }
}