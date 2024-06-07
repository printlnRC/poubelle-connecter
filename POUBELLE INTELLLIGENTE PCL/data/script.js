document.addEventListener('DOMContentLoaded', function() {
    function updateStatusPercentage() {
        fetch('/status')
            .then(response => response.text())
            .then(percentage => {
                document.getElementById('statusValue').innerText = percentage + '%';
            })
            .catch(error => {
                console.error('Error fetching status value:', error);
                document.getElementById('statusValue').innerText = 'Error';
            });
    }

    function updateStatusVolume() {
        fetch('/volume')
            .then(response => response.text())
            .then(volume => {
                document.getElementById('statusVolume').innerText = volume + 'm3';
            })
            .catch(error => {
                console.error('Error fetching volume value:', error);
                document.getElementById('statusVolume').innerText = 'Error';
            });
    }

    function updateStatusMasse() {
        fetch('/masse')
            .then(response => response.text())
            .then(masse => {
                document.getElementById('statusmasse').innerText = masse + 'kg';
            })
            .catch(error => {
                console.error('Error fetching masse value:', error);
                document.getElementById('statusmasse').innerText = 'Error';
            });
    }  
     function updateStatusBatterie() {
        fetch('/Batterie')  // Assurez-vous que cette route correspond à celle définie dans votre code Arduino
            .then(response => response.text())
            .then(bat => {
                document.getElementById('statusbat').innerText = bat + '%';
            })
            .catch(error => {
                console.error('Error fetching bat value:', error);
                document.getElementById('statusbat').innerText = 'Error';
            });
    }

    updateStatusPercentage();
    updateStatusVolume();
    updateStatusMasse();
    updateStatusBatterie();

    setInterval(updateStatusPercentage, 500);
    setInterval(updateStatusVolume, 500);
    setInterval(updateStatusMasse, 500);
    setInterval(updateStatusBatterie, 500);
});
