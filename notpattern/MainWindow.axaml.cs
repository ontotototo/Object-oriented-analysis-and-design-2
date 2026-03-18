using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Media.Imaging;
using Avalonia.Platform;
using System;
using System.Threading.Tasks;
using notpattern.Models;

namespace notpattern;

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

        var sneaker = new Sneaker();

        int steps = 1;

        if (GelCB.IsChecked == true) steps++;
        if (ReinforcedSoleCB.IsChecked == true) steps++;
        if (PremiumUpperCB.IsChecked == true) steps++;
        if (CollabCB.IsChecked == true) steps++;

        double stepSize = 100.0 / steps;


        if (VolleyballRB.IsChecked == true)
        {
            sneaker.Model = "ASICS Volleyball Pro";
            sneaker.Purpose = "Волейбол";
            sneaker.SoleType = "Резиновая";
        }
        else if (UrbanRB.IsChecked == true)
        {
            sneaker.Model = "ASICS Urban Walk";
            sneaker.Purpose = "Город";
            sneaker.SoleType = "Лёгкая";
        }
        else
        {
            sneaker.Model = "ASICS Limited";
            sneaker.Purpose = "Коллекционная";
            sneaker.SoleType = "Премиальная";
        }

        Log("Создана базовая модель");
        await AnimateStep(stepSize);

        if (GelCB.IsChecked == true)
        {
            sneaker.HasGel = true;
            Log("Добавлен гель");
            await AnimateStep(stepSize);
        }

        if (ReinforcedSoleCB.IsChecked == true)
        {
            sneaker.ReinforcedSole = true;
            Log("Усиленная подошва");
            await AnimateStep(stepSize);
        }

        if (PremiumUpperCB.IsChecked == true)
        {
            sneaker.PremiumUpper = true;
            Log("Премиальный верх");
            await AnimateStep(stepSize);
        }

        if (CollabCB.IsChecked == true &&
            CollabBrandBox.SelectedItem is ComboBoxItem item)
        {
            sneaker.Collaboration = item.Content!.ToString()!;
            Log($"Коллаборация с {item.Content}");
            await AnimateStep(stepSize);
        }

        ResultBox.Text = sneaker.GetDescription();

        Log("✅ Сборка завершена");
        Progress.Value = 100;

        ShowImage(sneaker);
    }

    private void Log(string text)
    {
        LogBox.Text += text + "\n";
    }

    private async Task AnimateStep(double step)
    {
        Progress.Value += step;
        await Task.Delay(300);
    }

    private void ShowImage(Sneaker s)
    {
        string path;

        if (s.Purpose == "Волейбол")
        {
            if (s.PremiumUpper && s.ReinforcedSole)
                path = "avares://notpattern/Assets/volley_premium.png";
            else if (s.PremiumUpper)
                path = "avares://notpattern/Assets/volley_premium.png";
            else if (s.ReinforcedSole)
                path = "avares://notpattern/Assets/volley_basic.png";
            else
                path = "avares://notpattern/Assets/volley_basic.png";
        }

        else if (s.Purpose == "Город")
        {
            if (s.PremiumUpper && s.HasGel)
                path = "avares://notpattern/Assets/urban_premium.png";
            else if (s.PremiumUpper)
                path = "avares://notpattern/Assets/urban_premium.png";
            else if (s.HasGel)
                path = "avares://notpattern/Assets/urban_premium.png";
            else
                path = "avares://notpattern/Assets/urban_basic.png";
        }

        else
        {
            path = "avares://notpattern/Assets/collab.png";
        }

        try
        {
            SneakerImage.Source =
                new Bitmap(AssetLoader.Open(new Uri(path)));
        }
        catch
        {
            Log(" Картинка не найдена");
        }
    }
}