# Overview
Interactive Reports are plugins that create a text-based window in Understand to display information of your choosing. They can reference the entire Project, a given Architecture, a specific entity or CodeCheck Results, and they can be as simple or complex as your imagination. There is an IReport that ships with Understand that shows all the API information available for the selected entity. To see it yourself just right click on an entity and select Interactive Reports->API Info  

After installation, project level reports can be run by selecting Project->Interactive Reports -> [Plugin Name]. Entity Specific reports can be run by Right Clicking on the entity and then selecting Interactive Reports -> [Plugin Name].

# Making Your Own Custom Reports
You can customize any of these reports, or make your own from scratch. We recommend starting with our [sample IReport plugin](https://docs.scitools.com/python/ireport.html), included for that purpose. The full Interactive Report API documentation is accessible in Understand by selecting Help->Python API Documentation and navigating to the IReports section. See also the [Interactive Reports tutorial](https://docs.scitools.com/help/apis/tutorials/tutorial-6-interactive-reports.html).
