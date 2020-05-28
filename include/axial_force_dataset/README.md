# Axial Force Dataset

This library parses .csv data files and .json metadata files that describe the 
axial loads during needle insertion into soft tissue. 

## JSON metadata file structure.
This section provies a brief descripion of the metadata file structure used 
for the dataset description. The sections of the the file are the following:

### Source Section
This section provides information with regard to the source of the dataset.
The variables used in this section are the following:
1. `Author`: &nbsp; Author's name. 

2. `Paper Title`: &nbsp; Paper title.

3. `Year`: &nbsp; Publication year.

4. `DOI`: &nbsp; Papers digital object identifier.

### Needle Characteristics Section
This section provides information with regard to the characteristics 
of the needle used during data collection. The variables used in this 
section are the following:

1. `Needle Diameter`: &nbsp; The diameter of the needle in gauge (G). 

2. `Tip Type`: &nbsp; The tipe of the needle's tip. Possible values are: &nbsp;
<code><i> "Blunt", "Beveled", "Conical", "Sprotte", "Diamond", "Tuohy", "Franseen", "Uknown". </i>
</code>

3. `Tip Angle`: &nbsp; The needle's tip angle in degrees.

4. `Tip Sharpness`: &nbsp; The needle's tip sharpness. Possible values are: &nbsp;
<code><i> "Sharp", "Blunt", "Uknown". </i> </code>

5. Tip Lubrication: The needle's lubrication state. Possible values are: &nbsp;
<code><i> "Present", "Not Present", "Uknown". </i> </code>


### Tissue Characteristics Section
This section provides information with regard to the characteristics 
of the tissue that was used during data collection. The variables used in this 
section are the following:

1. `Tissue Type`: &nbsp; Defines the type of tissue used during experiments.
Possible values are: &nbsp; <code><i> "Biological", "Artificial". </i> </code>

2. `Layers Number`: &nbsp; The number of material layers used during needle 
insertion.

4. `Tissue Description`: &nbsp; Describes the characteristics of tissue 
used. Base on the tissue type it can contain the following descriptions:
    1. <code> <b> Biological Tissue </b></code> <br />
        - `Organ/Location`: &nbsp; Defines the organ(s)/location(s) of insertion. Possible values are: &nbsp; <code><i> "Liver", "Prostate", "Breast", "Brain", "Heart", "Epidural", "Skin", "Muscle", "Fat". </i></code>
        - `Animal`: &nbsp; Defines the animal/specimen used for the needle insertion: &nbsp; <code><i> "Human", "Bovine specimen", "Porcine specimen", "Rabbit specimen", "Chicken specimen". </i></code>
        - `State`: &nbsp; Defines the state of the animal/specimen used for the needle insertion: &nbsp; <code><i> "In vivo", "Ex vivo". </i></code>
    2. <code> <b> Artificial Tissue </b></code> <br />
        - `Name`: &nbsp; Defines the name(s) of the artificial tissue(s) used 
        for needle insertion.
        - `Components`: &nbsp; Defines the components that the artificial tissue
        contatains.
        - `Concentation`: &nbsp; Defines the concentation(s) of the used 
        components.

### Measurements Section
This section provides information with regard to the measurements taken during 
needle insertion. The variables used in this section are the following:

1. `Sampling Frequency`: &nbsp; Defines the sampling frequency used for 
data collection. The frequency is given in Hz.

2. `Indepedent variables`: &nbsp;Defines the indepedent variables used during 
data collection. Possible values are: &nbsp; <code><i> "Time", "Displacement x",
"Velocity x", "Rotation x". </i></code>

3. `Depedent variables`: &nbsp;Defines the depedent variables measured during 
data collection. Possible values are: &nbsp; <code><i> "Time", "Displacement x",
"Velocity x", "Rotation x", "Force x". </i></code>

4. `File Names`: &nbsp;File names of the .csv files that contain the data.
Each file must contain to columns of values that correspond to the 
`Indepedent variables` and `Depedent variables` that were specified above.

5. `Constants`: &nbsp; Defines the quantities that are kept constant during 
needle insertion. Possible values are: &nbsp; <code><i> "Velocity x", 
"Rotation x". </i></code>

6. `Constant Values`: &nbsp; Defines the values of the constant quantities that
were specified above.

## Example of JSON metafile.
An example of a .json metafile is presented below.

```json
"Data0" : 
{
    "Source" : 
    {
        "Author": "Okamura, Simone", 
        "Paper Title": "Force modeling for needle insertion into soft tissue", 
        "Year": 2001, 
        "DOI": "10.1109/TBME.2004.831542"
    },

    "Needle Characteristics":
    {
        "Needle Diameter": 18, 
        "Tip Type": "Beveled", 
        "Tip Angle": 10, 
        "Tip Sharpness": "Uknown",
        "Tip Lubrication": "Uknown"
    },

    "Tissue Characteristics":
    {
        "Tissue Type": "Biological", 
        "Layers Number": 2,
        "Tissue Description":
        {
            "Organ/Location": ["Liver", "Heart"],
            "Animal": ["Bovine Specimen", "Bovine Specimen"],
            "State": ["Ex vivo", "Ex vivo"]
        }  
    },

    "Measurements":
    {
        "Sampling Frequency" : 1000,
        "Indepedent Variables": ["Displacement x"],
        "Depedent Variables": ["Force x"],
        "Files Names": ["depth_force.csv"],
        "Constants": ["Velocity x", "Rotation x"],
        "Constants Values": [0.003, 0.00]
    }
},
```

## Units  
Each of the variables defined above must be specified in specific units as 
they are presented in the following table:

|Quantity|Unit|
| ---| ---|
|`Needle Diameter`| Gauge|
| `Tip Angle` | Degrees |
| `Sampling Frequency` | Hertz |
| `Displacement x` | meters |
| `Velocity x` | meters / seconds |
| `Rotation x` | radians / seconds |
| `Force x` | Newtons |

## DATASET API
The dataset API parses the data defined by the DATA_ID: &nbsp; `Datai, i = 1,...,n`.

The following example illustrates the use of the API.

```cpp

    // Create a vector of dataset objects
    std::vector<std::string> data_vec = {"Data0", "Data1"};
    
    AxialForceDataset axial_data[2];

    for (int i = 0; i < sizeof(axial_data) / sizeof(axial_data[0]); i++)
    {
        // For each of the datasets parse the data
        axial_data[i].data_parsing(data_vec[i]);

        // For each of the datasets get the time and displacement vector
        arma::fvec time = axial_data[i].get_time();
        arma::fvec displacemt_x = axial_data[i].get_displ_x();

        // For each of the datasets check if it refers to biological tissue 
        // and extract its description
        if (axial_data[i].is_tissue_biological())
        {
            auto tissue_description = axial_data[i].get_tissue_description();
            int organ_index = axial_data[i].bio_tissue_organ_index;
            std::string organ = tissue_description[organ][1];
        }
    }
```