# Direct UXP data access without full decryption

The following files are used to demonstrate direct access to protected data.  They utilize the UXL Script Engine; however, the behavior can be replicated using the native UXL API.

- [Workflow.data]()
  - Sample data ... a file of transaction records that can be accessed by record #.
- Workflow.kcl
  - Native KCL file used to create a UXP.
- WorkflowClear.data
  - Sample data ... unprotected.
- WorkflowCreate.uxl
  - Script to create a UXP using the Workflow KCL script.
- WorkflowCreateRun.uxl
  - Script to create a UXP using the Workflow KCL script.  Also starts the application.
- WorkflowDataMultiplier.uxl
  - Creates a version of WorkflowClear.data using the multiplier.
- WorkflowIndex.csv
  - Index into protected data.
- WorkflowRun.uxl
  - Script to run application against protected data.

The base Workflow.data contains 501 records.  By utilizing a multiplier, one can create a sample data
set that is 'N' times larger than the original sample.  For example, by using a multiplier of 2, the
resultant sample set will contain 1002 records.

1. To get started, run the SmartFile Script Engine.
2. At the prompt, type: x::cd("the folder containing the above workflow demo files");
3. To create protected data, at the prompt, type: @WorkflowCreate.uxl
4. To run the application, at the prompt, type: @WorkflowRun.uxl

This will prompt for a multiplier, which must be less than or equal to the multiplier established when creating the sample data.

The next prompt will be a loop, where the application will prompt for a record number.  It will extract the
selected record from within the UXP exactly like an application would fetch data from an unprotected clear
data file.

